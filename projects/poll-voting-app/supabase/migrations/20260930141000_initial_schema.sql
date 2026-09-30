-- Phase 1: Initial secure schema for Poll & Voting application
-- Run in Supabase SQL Editor

create extension if not exists pgcrypto;

create type public.user_role as enum ('user', 'admin');
create type public.poll_status as enum ('draft', 'scheduled', 'active', 'closed');
create type public.results_visibility as enum ('always', 'after_close', 'never');

create table if not exists public.profiles (
  id uuid primary key references auth.users(id) on delete cascade,
  name text not null check (char_length(trim(name)) between 2 and 120),
  email text not null unique,
  role public.user_role not null default 'user',
  created_at timestamptz not null default timezone('utc', now())
);

create table if not exists public.polls (
  id uuid primary key default gen_random_uuid(),
  title text not null check (char_length(trim(title)) between 3 and 200),
  description text not null default '' check (char_length(description) <= 5000),
  status public.poll_status not null default 'draft',
  start_time timestamptz not null,
  end_time timestamptz not null,
  results_visibility public.results_visibility not null default 'after_close',
  created_by uuid not null references public.profiles(id) on delete restrict,
  created_at timestamptz not null default timezone('utc', now()),
  updated_at timestamptz not null default timezone('utc', now()),
  constraint polls_time_window_check check (end_time > start_time)
);

create table if not exists public.poll_options (
  id uuid primary key default gen_random_uuid(),
  poll_id uuid not null references public.polls(id) on delete cascade,
  option_text text not null check (char_length(trim(option_text)) between 1 and 300),
  display_order integer not null check (display_order >= 0),
  created_at timestamptz not null default timezone('utc', now()),
  unique (poll_id, display_order),
  unique (poll_id, id)
);

create table if not exists public.votes (
  id uuid primary key default gen_random_uuid(),
  poll_id uuid not null references public.polls(id) on delete cascade,
  option_id uuid not null,
  user_id uuid not null references public.profiles(id) on delete cascade,
  created_at timestamptz not null default timezone('utc', now()),
  constraint votes_option_poll_fk foreign key (poll_id, option_id)
    references public.poll_options (poll_id, id) on delete restrict,
  constraint votes_one_per_user_per_poll unique (poll_id, user_id)
);

create table if not exists public.security_events (
  id uuid primary key default gen_random_uuid(),
  user_id uuid references public.profiles(id) on delete set null,
  poll_id uuid references public.polls(id) on delete set null,
  event_type text not null check (char_length(trim(event_type)) between 3 and 120),
  metadata jsonb not null default '{}'::jsonb,
  created_at timestamptz not null default timezone('utc', now())
);

create index if not exists idx_profiles_role on public.profiles(role);
create index if not exists idx_polls_status_time on public.polls(status, start_time, end_time);
create index if not exists idx_poll_options_poll_id on public.poll_options(poll_id);
create index if not exists idx_votes_poll_id on public.votes(poll_id);
create index if not exists idx_votes_user_id on public.votes(user_id);
create index if not exists idx_security_events_created_at on public.security_events(created_at desc);
create index if not exists idx_security_events_event_type on public.security_events(event_type);

create or replace function public.set_updated_at()
returns trigger
language plpgsql
as $$
begin
  new.updated_at = timezone('utc', now());
  return new;
end;
$$;

drop trigger if exists set_polls_updated_at on public.polls;
create trigger set_polls_updated_at
before update on public.polls
for each row
execute function public.set_updated_at();

create or replace function public.handle_new_user()
returns trigger
language plpgsql
security definer
set search_path = public
as $$
begin
  insert into public.profiles (id, name, email)
  values (
    new.id,
    coalesce(nullif(trim(new.raw_user_meta_data ->> 'name'), ''), split_part(new.email, '@', 1)),
    new.email
  )
  on conflict (id) do update
    set name = excluded.name,
        email = excluded.email;

  return new;
end;
$$;

drop trigger if exists on_auth_user_created on auth.users;
create trigger on_auth_user_created
after insert on auth.users
for each row
execute function public.handle_new_user();

create or replace function public.is_admin(check_user_id uuid)
returns boolean
language sql
stable
security definer
set search_path = public
as $$
  select exists (
    select 1
    from public.profiles p
    where p.id = check_user_id and p.role = 'admin'
  );
$$;

create or replace function public.prevent_role_escalation()
returns trigger
language plpgsql
security definer
set search_path = public
as $$
begin
  if new.role is distinct from old.role
    and auth.role() <> 'service_role'
    and not public.is_admin(auth.uid()) then
    raise exception 'Only administrators can change user roles';
  end if;

  return new;
end;
$$;

drop trigger if exists prevent_role_escalation on public.profiles;
create trigger prevent_role_escalation
before update on public.profiles
for each row
execute function public.prevent_role_escalation();

alter table public.profiles enable row level security;
alter table public.polls enable row level security;
alter table public.poll_options enable row level security;
alter table public.votes enable row level security;
alter table public.security_events enable row level security;

-- Profiles policies
create policy "profiles_select_own_or_admin"
on public.profiles
for select
using (auth.uid() = id or public.is_admin(auth.uid()));

create policy "profiles_insert_own"
on public.profiles
for insert
with check (auth.uid() = id);

create policy "profiles_update_own"
on public.profiles
for update
using (auth.uid() = id or public.is_admin(auth.uid()))
with check (auth.uid() = id or public.is_admin(auth.uid()));

-- Poll policies
create policy "polls_select_visible_or_admin"
on public.polls
for select
using (
  status in ('scheduled', 'active', 'closed')
  or public.is_admin(auth.uid())
);

create policy "polls_admin_manage"
on public.polls
for all
using (public.is_admin(auth.uid()))
with check (public.is_admin(auth.uid()));

-- Poll options policies
create policy "poll_options_select_visible_or_admin"
on public.poll_options
for select
using (
  exists (
    select 1
    from public.polls p
    where p.id = poll_options.poll_id
      and (p.status in ('scheduled', 'active', 'closed') or public.is_admin(auth.uid()))
  )
);

create policy "poll_options_admin_manage"
on public.poll_options
for all
using (public.is_admin(auth.uid()))
with check (public.is_admin(auth.uid()));

-- Votes policies
create policy "votes_select_own_or_admin"
on public.votes
for select
using (auth.uid() = user_id or public.is_admin(auth.uid()));

create policy "votes_insert_own_once_active_poll"
on public.votes
for insert
with check (
  auth.uid() = user_id
  and exists (
    select 1
    from public.polls p
    where p.id = votes.poll_id
      and p.status = 'active'
      and timezone('utc', now()) between p.start_time and p.end_time
  )
  and exists (
    select 1
    from public.poll_options po
    where po.id = votes.option_id and po.poll_id = votes.poll_id
  )
  and not exists (
    select 1
    from public.votes v
    where v.poll_id = votes.poll_id and v.user_id = auth.uid()
  )
);

-- Security event policies
create policy "security_events_admin_read"
on public.security_events
for select
using (public.is_admin(auth.uid()));

create policy "security_events_admin_insert"
on public.security_events
for insert
with check (public.is_admin(auth.uid()));

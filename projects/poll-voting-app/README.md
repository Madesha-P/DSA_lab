# Poll & Voting Web Application

This folder contains the new project at:

`/home/runner/work/DSA_lab/DSA_lab/projects/poll-voting-app`

## Phase 1 Scope (Completed)

- Next.js (App Router) + TypeScript + Tailwind CSS project setup
- Initial project structure for components/lib/types
- Supabase schema migration with:
  - `profiles`, `polls`, `poll_options`, `votes`, `security_events`
  - Primary/foreign keys and indexes
  - Unique vote rule: `UNIQUE (poll_id, user_id)`
  - Poll-option ownership validation via composite FK
  - RLS enabled and baseline policies
  - User profile auto-provisioning trigger from `auth.users`
- Environment variable template in `.env.example`

## Run Locally

```bash
npm install
npm run dev
```

Open `http://localhost:3000`.

## Supabase Setup (Exact Click-by-Click)

### Step 1: Create Supabase project
1. Open: **https://supabase.com/dashboard**.
2. Click: **New project**.
3. Enter:
   - Organization
   - Project name: `poll-voting-app`
   - Database password (strong password)
   - Region closest to you
4. Click: **Create new project**.
5. Expected result: project dashboard opens after provisioning.
6. Verify: you can see left menu items like **Table Editor**, **SQL Editor**, **Authentication**.

### Step 2: Run database migration SQL
1. In your project, open: **SQL Editor**.
2. Click: **New query**.
3. Open file:
   - `/home/runner/work/DSA_lab/DSA_lab/projects/poll-voting-app/supabase/migrations/20260930141000_initial_schema.sql`
4. Copy all SQL and paste into the editor.
5. Click: **Run**.
6. Expected result: query completes successfully with no errors.
7. Verify:
   - Open **Table Editor** and confirm tables exist: `profiles`, `polls`, `poll_options`, `votes`, `security_events`.
   - Open **Authentication > Users** later after sign-up; new user should get a profile row.

### Step 3: Confirm RLS is enabled
1. Open: **Table Editor**.
2. Click table `votes`.
3. Open tab: **Policies**.
4. Expected result: policies are listed and RLS is enabled.
5. Verify: repeat for `profiles`, `polls`, `poll_options`, `security_events`.

### Step 4: Get API keys and URL
1. Open: **Project Settings** (gear icon) > **API**.
2. Copy:
   - **Project URL**
   - **anon public key**
   - **service_role key** (server-only)
3. In local project, create `.env.local` from `.env.example`.
4. Enter copied values:
   - `NEXT_PUBLIC_SUPABASE_URL`
   - `NEXT_PUBLIC_SUPABASE_ANON_KEY`
   - `SERVER_ONLY_SUPABASE_SERVICE_ROLE_KEY`
5. Expected result: app has credentials for next phases.
6. Verify: values are present in `.env.local` and app starts with `npm run dev`.

## Important Security Notes (Phase 1)

- Never expose `SERVER_ONLY_SUPABASE_SERVICE_ROLE_KEY` in client-side code.
- Do not commit `.env.local`.
- Vote duplication is blocked at DB layer via `UNIQUE (poll_id, user_id)`.
- Vote option/poll mismatch is blocked by foreign key + RLS checks.

## Files Added/Updated in Phase 1

- `app/layout.tsx`
- `app/page.tsx`
- `.env.example`
- `supabase/migrations/20260930141000_initial_schema.sql`
- `components/**/.gitkeep`
- `lib/**/.gitkeep`
- `types/.gitkeep`

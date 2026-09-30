# Poll & Voting Web Application

Project path:
`/home/runner/work/DSA_lab/DSA_lab/projects/poll-voting-app`

## Completed so far

### Phase 1
- Next.js + TypeScript + Tailwind scaffold
- Supabase schema migration with tables, constraints, triggers, and RLS
- Environment template and project structure

### Phase 2
- Supabase Auth server integration for Next.js
- Secure auth API routes with Zod validation
- Login/Register/Forgot Password pages
- Session-aware middleware for protected routes (`/dashboard`, `/admin`)
- Security event logging helper + rate-limit integration points

## Phase 2 files added/updated

- `app/api/auth/register/route.ts`
- `app/api/auth/login/route.ts`
- `app/api/auth/logout/route.ts`
- `app/api/auth/forgot-password/route.ts`
- `app/auth/callback/route.ts`
- `app/login/page.tsx`
- `app/register/page.tsx`
- `app/forgot-password/page.tsx`
- `app/dashboard/page.tsx`
- `middleware.ts`
- `lib/validation/auth.ts`
- `lib/auth/session.ts`
- `lib/security/rate-limit.ts`
- `lib/security/events.ts`
- `.env.example`
- `.gitignore`

## Local run

```bash
npm install
npm run dev
```

## Exact Supabase actions for Phase 2

### 1) Configure Auth redirect URLs
1. Open: **https://supabase.com/dashboard**.
2. Click: your project.
3. Open: **Authentication** > **URL Configuration**.
4. In **Site URL**, enter: `http://localhost:3000`.
5. In **Redirect URLs**, click **Add URL** and enter:
   - `http://localhost:3000/auth/callback`
6. Click: **Save**.
7. Expected result: URL settings save successfully.
8. Verify: the callback URL appears in the redirect URL list.

### 2) Enable email/password provider
1. Open: **Authentication** > **Providers**.
2. Click: **Email** provider.
3. Ensure **Enable Email provider** is ON.
4. (Recommended while learning) keep **Confirm email** ON.
5. Click: **Save**.
6. Expected result: Email provider shows enabled state.
7. Verify: registration sends verification flow instead of immediate failure.

### 3) Create local environment file
1. In your local project folder, copy `.env.example` to `.env.local`.
2. Open Supabase: **Project Settings** > **API**.
3. Copy and enter into `.env.local`:
   - `NEXT_PUBLIC_SUPABASE_URL`
   - `NEXT_PUBLIC_SUPABASE_ANON_KEY`
   - `SERVER_ONLY_SUPABASE_SERVICE_ROLE_KEY`
4. Set `NEXT_PUBLIC_APP_URL=http://localhost:3000`.
5. (Optional for rate limiting) add Upstash values:
   - `UPSTASH_REDIS_REST_URL`
   - `UPSTASH_REDIS_REST_TOKEN`
6. Expected result: app has required credentials.
7. Verify: `npm run dev` starts without missing-env auth errors.

### 4) Test registration flow
1. Open: `http://localhost:3000/register`.
2. Enter: name, email, strong password, confirm password.
3. Click: **Register**.
4. Expected result: success message appears.
5. Verify in Supabase:
   - Open **Authentication** > **Users**: new user exists.
   - Open **Table Editor** > `profiles`: row auto-created by trigger.

### 5) Test login flow
1. Open: `http://localhost:3000/login`.
2. Enter registered email/password.
3. Click: **Login**.
4. Expected result: redirect to `/dashboard`.
5. Verify: dashboard loads and shows authenticated email.

### 6) Test route protection
1. Open browser in logged-out state (or incognito).
2. Visit: `http://localhost:3000/dashboard`.
3. Expected result: redirected to `/login`.
4. Verify: protected route is not accessible without session.

## Security considerations implemented in Phase 2

- All auth writes happen through server-side route handlers.
- Request bodies are validated with Zod before Supabase calls.
- Auth endpoints include rate-limit checks (when Upstash is configured).
- Session checks are enforced server-side in middleware and dashboard loader.
- Security events are written without storing passwords or tokens.
- Service role key is server-only and never used in client-side code.

## Notes

- If Upstash env vars are not set, auth still works and rate limiting is bypassed in development mode.
- This phase establishes secure auth foundations; poll listing/voting flows will be added in the next phases.

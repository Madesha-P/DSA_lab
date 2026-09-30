import Link from "next/link";

export default function Home() {
  return (
    <main className="flex min-h-screen flex-col items-center justify-center bg-slate-950 px-6 py-16 text-slate-100">
      <div className="w-full max-w-3xl rounded-2xl border border-slate-800 bg-slate-900/70 p-8 shadow-2xl backdrop-blur">
        <p className="text-sm font-medium uppercase tracking-[0.2em] text-emerald-400">
          Phase 1 Complete
        </p>
        <h1 className="mt-3 text-3xl font-semibold tracking-tight sm:text-4xl">
          Poll &amp; Voting Platform Setup
        </h1>
        <p className="mt-4 text-base text-slate-300">
          Next.js + TypeScript + Tailwind project scaffolding and secure Supabase schema migration are now ready.
        </p>
        <ul className="mt-8 space-y-3 text-sm text-slate-300">
          <li>• Project scaffolded in App Router mode.</li>
          <li>• Supabase migration includes tables, constraints, triggers, and RLS policies.</li>
          <li>• Environment variable template is included in .env.example.</li>
        </ul>
        <div className="mt-8 flex flex-wrap gap-3">
          <Link href="/login" className="rounded-lg bg-emerald-600 px-4 py-2 text-sm font-medium text-white hover:bg-emerald-500">
            Login
          </Link>
          <Link href="/register" className="rounded-lg border border-slate-700 px-4 py-2 text-sm font-medium text-slate-100 hover:bg-slate-800">
            Register
          </Link>
        </div>
      </div>
    </main>
  );
}

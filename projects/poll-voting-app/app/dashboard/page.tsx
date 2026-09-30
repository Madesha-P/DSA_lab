import { requireUser } from "@/lib/auth/session";

export default async function DashboardPage() {
  const user = await requireUser();

  return (
    <main className="mx-auto flex min-h-screen w-full max-w-3xl flex-col px-6 py-16">
      <h1 className="text-3xl font-semibold">Dashboard</h1>
      <p className="mt-3 text-slate-600 dark:text-slate-300">Welcome, {user.email}. Authentication is active and your session is protected server-side.</p>
      <p className="mt-6 rounded-lg border border-slate-300 bg-slate-50 p-4 text-sm dark:border-slate-700 dark:bg-slate-900">
        Next phase will load active, upcoming, and closed polls here.
      </p>
    </main>
  );
}

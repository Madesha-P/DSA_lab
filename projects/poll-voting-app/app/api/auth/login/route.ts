import { NextResponse } from "next/server";
import { createSupabaseServerClient } from "@/lib/supabase/server";
import { logSecurityEvent } from "@/lib/security/events";
import { checkRateLimit } from "@/lib/security/rate-limit";
import { loginSchema } from "@/lib/validation/auth";

export async function POST(request: Request) {
  const rateLimit = await checkRateLimit(request, "auth-login");

  if (!rateLimit.success) {
    await logSecurityEvent({ eventType: "rate_limit_login_blocked" });

    return NextResponse.json(
      { error: "Too many login attempts. Please wait and try again." },
      { status: 429 },
    );
  }

  const requestData = await request.json().catch(() => null);
  const validation = loginSchema.safeParse(requestData);

  if (!validation.success) {
    return NextResponse.json(
      {
        error: "Invalid login request.",
        details: validation.error.flatten().fieldErrors,
      },
      { status: 400 },
    );
  }

  const supabase = await createSupabaseServerClient();
  const { email, password } = validation.data;
  const { data, error } = await supabase.auth.signInWithPassword({ email, password });

  if (error || !data.user) {
    await logSecurityEvent({
      eventType: "login_failed",
      metadata: { email_domain: email.split("@")[1] ?? null },
    });

    return NextResponse.json(
      { error: "Invalid email or password." },
      { status: 401 },
    );
  }

  await logSecurityEvent({ eventType: "login_success", userId: data.user.id });

  return NextResponse.json({ message: "Login successful." });
}

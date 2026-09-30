import { NextResponse } from "next/server";
import { createSupabaseServerClient } from "@/lib/supabase/server";
import { logSecurityEvent } from "@/lib/security/events";
import { checkRateLimit } from "@/lib/security/rate-limit";
import { registerSchema } from "@/lib/validation/auth";

export async function POST(request: Request) {
  const rateLimit = await checkRateLimit(request, "auth-register");

  if (!rateLimit.success) {
    await logSecurityEvent({ eventType: "rate_limit_register_blocked" });

    return NextResponse.json(
      { error: "Too many registration attempts. Please wait and try again." },
      { status: 429 },
    );
  }

  const requestData = await request.json().catch(() => null);
  const validation = registerSchema.safeParse(requestData);

  if (!validation.success) {
    return NextResponse.json(
      {
        error: "Invalid registration details.",
        details: validation.error.flatten().fieldErrors,
      },
      { status: 400 },
    );
  }

  const { name, email, password } = validation.data;
  const appUrl = process.env.NEXT_PUBLIC_APP_URL || "http://localhost:3000";

  const supabase = await createSupabaseServerClient();
  const { data, error } = await supabase.auth.signUp({
    email,
    password,
    options: {
      data: { name },
      emailRedirectTo: `${appUrl}/auth/callback?next=/dashboard`,
    },
  });

  if (error) {
    await logSecurityEvent({
      eventType: "register_failed",
      metadata: { message: error.message },
    });

    return NextResponse.json(
      { error: "Registration failed. Please check your details and try again." },
      { status: 400 },
    );
  }

  await logSecurityEvent({ eventType: "register_success", userId: data.user?.id });

  return NextResponse.json({
    message:
      data.session === null
        ? "Registration successful. Please verify your email before signing in."
        : "Registration successful.",
  });
}

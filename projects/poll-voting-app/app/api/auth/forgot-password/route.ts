import { NextResponse } from "next/server";
import { createSupabaseServerClient } from "@/lib/supabase/server";
import { logSecurityEvent } from "@/lib/security/events";
import { checkRateLimit } from "@/lib/security/rate-limit";
import { forgotPasswordSchema } from "@/lib/validation/auth";

export async function POST(request: Request) {
  const rateLimit = await checkRateLimit(request, "auth-forgot-password");

  if (!rateLimit.success) {
    await logSecurityEvent({ eventType: "rate_limit_password_reset_blocked" });

    return NextResponse.json(
      { error: "Too many password reset attempts. Please wait and try again." },
      { status: 429 },
    );
  }

  const requestData = await request.json().catch(() => null);
  const validation = forgotPasswordSchema.safeParse(requestData);

  if (!validation.success) {
    return NextResponse.json(
      {
        error: "Invalid request.",
        details: validation.error.flatten().fieldErrors,
      },
      { status: 400 },
    );
  }

  const appUrl = process.env.NEXT_PUBLIC_APP_URL || "http://localhost:3000";
  const supabase = await createSupabaseServerClient();

  const { error } = await supabase.auth.resetPasswordForEmail(validation.data.email, {
    redirectTo: `${appUrl}/auth/callback?next=/dashboard`,
  });

  if (error) {
    await logSecurityEvent({
      eventType: "password_reset_failed",
      metadata: { message: error.message },
    });
  } else {
    await logSecurityEvent({
      eventType: "password_reset_requested",
      metadata: { email_domain: validation.data.email.split("@")[1] ?? null },
    });
  }

  return NextResponse.json({
    message: "If an account exists for this email, a reset link has been sent.",
  });
}

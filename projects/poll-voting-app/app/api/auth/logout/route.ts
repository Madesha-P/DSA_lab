import { NextResponse } from "next/server";
import { createSupabaseServerClient } from "@/lib/supabase/server";
import { logSecurityEvent } from "@/lib/security/events";

export async function POST() {
  const supabase = await createSupabaseServerClient();
  const { data } = await supabase.auth.getUser();

  const { error } = await supabase.auth.signOut();

  if (error) {
    return NextResponse.json({ error: "Logout failed. Please try again." }, { status: 400 });
  }

  await logSecurityEvent({ eventType: "logout_success", userId: data.user?.id });

  return NextResponse.json({ message: "Logged out." });
}

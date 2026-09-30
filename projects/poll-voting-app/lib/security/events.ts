import { createSupabaseAdminClient } from "@/lib/supabase/admin";

type SecurityEventPayload = {
  eventType: string;
  userId?: string | null;
  pollId?: string | null;
  metadata?: Record<string, unknown>;
};

export async function logSecurityEvent(payload: SecurityEventPayload) {
  try {
    const adminClient = createSupabaseAdminClient();

    await adminClient.from("security_events").insert({
      event_type: payload.eventType,
      user_id: payload.userId ?? null,
      poll_id: payload.pollId ?? null,
      metadata: payload.metadata ?? {},
    });
  } catch {
    // Do not fail auth flows if audit logging fails.
  }
}

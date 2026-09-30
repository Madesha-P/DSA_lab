import { Ratelimit } from "@upstash/ratelimit";
import { Redis } from "@upstash/redis";

const redisUrl = process.env.UPSTASH_REDIS_REST_URL;
const redisToken = process.env.UPSTASH_REDIS_REST_TOKEN;

const ratelimit =
  redisUrl && redisToken
    ? new Ratelimit({
        redis: new Redis({ url: redisUrl, token: redisToken }),
        limiter: Ratelimit.slidingWindow(10, "1 m"),
        analytics: true,
      })
    : null;

function getRequestIp(request: Request) {
  const forwardedFor = request.headers.get("x-forwarded-for");
  if (forwardedFor) {
    return forwardedFor.split(",")[0]?.trim() || "unknown";
  }

  return request.headers.get("x-real-ip") || "unknown";
}

export async function checkRateLimit(request: Request, scope: string) {
  if (!ratelimit) {
    return { success: true as const, remaining: null };
  }

  const ip = getRequestIp(request);
  const { success, remaining, reset } = await ratelimit.limit(`${scope}:${ip}`);

  return { success, remaining, reset };
}

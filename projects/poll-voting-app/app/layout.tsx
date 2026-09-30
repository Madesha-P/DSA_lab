import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: "Poll & Voting App",
  description: "Secure online poll and voting platform",
};

export default function RootLayout({ children }: LayoutProps<"/">) {
  return (
    <html lang="en" className="h-full antialiased">
      <body className="min-h-full flex flex-col">{children}</body>
    </html>
  );
}

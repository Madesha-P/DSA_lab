(() => {
  const { SUPABASE_URL, SUPABASE_ANON_KEY } = window.TERMINAL_CHAT_CONFIG || {};

  const terminalOutput = document.getElementById("terminal-output");
  const terminalForm = document.getElementById("terminal-form");
  const terminalInput = document.getElementById("terminal-input");

  const MAX_MESSAGE_LENGTH = 500;
  const MIN_INTERVAL_MS = 1200;
  const INITIAL_LIMIT = 200;

  const messageElements = new Map();
  const commandHistory = [];
  let historyIndex = -1;
  let lastSentAt = 0;
  let realtimeChannel = null;

  function addLine(text, variant = "default") {
    const paragraph = document.createElement("p");
    paragraph.className = `line${variant === "default" ? "" : ` ${variant}`}`;
    paragraph.textContent = text;
    terminalOutput.append(paragraph);
  }

  function clearTerminalDisplay() {
    terminalOutput.textContent = "";
    messageElements.clear();
  }

  function printBootScreen() {
    addLine("Terminal Chat v1.0");
    addLine("────────────────────────────────────────", "muted");
    addLine("");
    addLine("Initializing terminal...", "muted");
    addLine("[OK] Interface initialized", "muted");
    addLine("[OK] Connecting to chat server...", "muted");
  }

  function printIntroPrompt() {
    addLine("");
    addLine("Public anonymous chat", "muted");
    addLine("");
    addLine("Type /help for available commands.", "muted");
    addLine("");
  }

  function formatTimestamp(isoString) {
    const date = new Date(isoString);
    const now = new Date();

    const isSameDay =
      date.getFullYear() === now.getFullYear() &&
      date.getMonth() === now.getMonth() &&
      date.getDate() === now.getDate();

    const timePart = date.toLocaleTimeString([], {
      hour: "2-digit",
      minute: "2-digit",
      hour12: false,
    });

    if (isSameDay) {
      return `[${timePart}]`;
    }

    const datePart = date.toLocaleDateString([], {
      day: "2-digit",
      month: "short",
    });

    return `[${datePart} ${timePart}]`;
  }

  function escapeMessage(raw) {
    return raw.trim();
  }

  function shouldAutoScroll() {
    const distanceFromBottom =
      terminalOutput.scrollHeight - terminalOutput.scrollTop - terminalOutput.clientHeight;
    return distanceFromBottom < 100;
  }

  function scrollToBottomIfNeeded(force = false) {
    if (force || shouldAutoScroll()) {
      terminalOutput.scrollTop = terminalOutput.scrollHeight;
    }
  }

  function renderStoredMessage(row, { forceScroll = false } = {}) {
    if (!row || typeof row.id === "undefined" || messageElements.has(row.id)) {
      return;
    }

    const line = document.createElement("p");
    line.className = "line";
    line.dataset.messageId = String(row.id);
    line.textContent = `${formatTimestamp(row.created_at)} ${row.message}`;

    terminalOutput.append(line);
    messageElements.set(row.id, line);
    scrollToBottomIfNeeded(forceScroll);
  }

  function removeMessageById(id) {
    const existing = messageElements.get(id);
    if (!existing) {
      return;
    }

    existing.remove();
    messageElements.delete(id);
  }

  function printHelp() {
    addLine("");
    addLine("Available commands:");
    addLine("");
    addLine("/help       Show available commands");
    addLine("/clear      Clear terminal display");
    addLine("/about      Show application information");
    addLine("");
  }

  function printAbout() {
    addLine("");
    addLine("Terminal Chat");
    addLine("Version: 1.0");
    addLine("Backend: Supabase");
    addLine("Hosting: GitHub Pages");
    addLine("Realtime: Supabase Realtime");
    addLine("Mode: Anonymous");
    addLine("");
  }

  function showConnectionStatus(state) {
    if (state === "connected") {
      addLine("[CONNECTED] Public chat server", "muted");
      return;
    }

    if (state === "reconnected") {
      addLine("[CONNECTED] Connection restored.", "muted");
      return;
    }

    addLine("[DISCONNECTED] Attempting to reconnect...", "error");
  }

  async function loadExistingMessages(supabase) {
    const { data, error } = await supabase
      .from("messages")
      .select("id,message,created_at")
      .order("created_at", { ascending: true })
      .limit(INITIAL_LIMIT);

    if (error) {
      addLine("[ERROR] Unable to load message history.", "error");
      return;
    }

    data.forEach((row) => renderStoredMessage(row, { forceScroll: false }));
    scrollToBottomIfNeeded(true);
  }

  async function sendMessageToSupabase(supabase, message) {
    const now = Date.now();
    if (now - lastSentAt < MIN_INTERVAL_MS) {
      addLine("[ERROR] Slow down. Please wait before sending another message.", "error");
      return;
    }

    const trimmed = escapeMessage(message);
    if (!trimmed) {
      return;
    }

    if (trimmed.length > MAX_MESSAGE_LENGTH) {
      addLine(`[ERROR] Message must be ${MAX_MESSAGE_LENGTH} characters or fewer.`, "error");
      return;
    }

    lastSentAt = now;

    const { error } = await supabase.from("messages").insert({ message: trimmed });

    if (error) {
      if (error.code === "429" || String(error.message || "").includes("rate")) {
        addLine("[ERROR] Rate limit hit. Please wait and try again.", "error");
      } else {
        addLine("[ERROR] Unable to send message.", "error");
      }
      lastSentAt = 0;
    }
  }

  function handleCommand(commandText) {
    const command = commandText.toLowerCase();

    if (command === "/help") {
      printHelp();
      return true;
    }

    if (command === "/about") {
      printAbout();
      return true;
    }

    if (command === "/clear") {
      clearTerminalDisplay();
      addLine("Terminal Chat v1.0");
      addLine("────────────────────────────────────────", "muted");
      addLine("");
      addLine("[OK] Connected", "muted");
      addLine("");
      return true;
    }

    addLine(`[ERROR] Unknown command: ${commandText}`, "error");
    addLine("");
    addLine("Type /help to see available commands.", "muted");
    addLine("");
    return true;
  }

  function pushHistory(entry) {
    if (!entry) {
      return;
    }

    commandHistory.push(entry);
    if (commandHistory.length > 30) {
      commandHistory.shift();
    }
    historyIndex = commandHistory.length;
  }

  function setupHistoryNavigation() {
    terminalInput.addEventListener("keydown", (event) => {
      if (event.key === "ArrowUp") {
        if (commandHistory.length === 0) {
          return;
        }
        event.preventDefault();
        historyIndex = Math.max(0, historyIndex - 1);
        terminalInput.value = commandHistory[historyIndex] || "";
        return;
      }

      if (event.key === "ArrowDown") {
        if (commandHistory.length === 0) {
          return;
        }
        event.preventDefault();
        historyIndex = Math.min(commandHistory.length, historyIndex + 1);
        terminalInput.value = commandHistory[historyIndex] || "";
      }
    });
  }

  async function setupRealtime(supabase) {
    realtimeChannel = supabase
      .channel("public:messages")
      .on(
        "postgres_changes",
        {
          event: "INSERT",
          schema: "public",
          table: "messages",
        },
        (payload) => {
          renderStoredMessage(payload.new, { forceScroll: false });
        },
      )
      .on(
        "postgres_changes",
        {
          event: "DELETE",
          schema: "public",
          table: "messages",
        },
        (payload) => {
          const id = payload.old?.id;
          if (typeof id !== "undefined") {
            removeMessageById(id);
            scrollToBottomIfNeeded(false);
          }
        },
      )
      .subscribe((status) => {
        if (status === "SUBSCRIBED") {
          showConnectionStatus("connected");
          return;
        }

        if (status === "CHANNEL_ERROR" || status === "TIMED_OUT" || status === "CLOSED") {
          showConnectionStatus("disconnected");
          return;
        }

        if (status === "REJOINED") {
          showConnectionStatus("reconnected");
        }
      });
  }

  function bindConnectivityEvents() {
    window.addEventListener("offline", () => {
      showConnectionStatus("disconnected");
    });

    window.addEventListener("online", () => {
      showConnectionStatus("reconnected");
    });

    document.addEventListener("visibilitychange", () => {
      if (!document.hidden) {
        terminalInput.focus();
      }
    });
  }

  function bindMessageInput(supabase) {
    terminalForm.addEventListener("submit", async (event) => {
      event.preventDefault();

      const rawValue = terminalInput.value;
      const value = rawValue.trim();

      if (!value) {
        terminalInput.value = "";
        return;
      }

      pushHistory(value);
      terminalInput.value = "";

      if (value.startsWith("/")) {
        handleCommand(value);
        scrollToBottomIfNeeded(true);
        return;
      }

      await sendMessageToSupabase(supabase, value);
      scrollToBottomIfNeeded(false);
    });
  }

  async function start() {
    if (!terminalOutput || !terminalForm || !terminalInput) {
      return;
    }

    printBootScreen();

    if (!SUPABASE_URL || !SUPABASE_ANON_KEY || SUPABASE_URL === "YOUR_SUPABASE_URL") {
      addLine("[ERROR] Supabase is not configured.", "error");
      addLine("Set SUPABASE_URL and SUPABASE_ANON_KEY in config.js", "muted");
      return;
    }

    const { createClient } = window.supabase;
    const supabase = createClient(SUPABASE_URL, SUPABASE_ANON_KEY);

    addLine("[OK] Connected", "muted");
    printIntroPrompt();

    try {
      await loadExistingMessages(supabase);
      await setupRealtime(supabase);
      bindMessageInput(supabase);
      setupHistoryNavigation();
      bindConnectivityEvents();
    } catch {
      addLine("[ERROR] Failed to initialize terminal chat.", "error");
    }

    terminalInput.focus();
  }

  window.addEventListener("beforeunload", () => {
    if (realtimeChannel && window.supabase) {
      const { removeChannel } = window.supabase;
      removeChannel(realtimeChannel);
    }
  });

  start();
})();

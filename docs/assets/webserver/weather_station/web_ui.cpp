#include "web_ui.h"

#include <WiFi.h>
#include <WebServer.h>

static WebServer server(80);
static WebSensorData gData = {0};
static String gApIp = "192.168.4.1";

// ── 서버사이드 히스토리 ──────────────────────────────────────
struct HistoryEntry { float temp; float humidity; float pressure; float altitude; uint32_t ms; };
static HistoryEntry gHistory[HISTORY_SIZE];
static uint16_t gHistHead = 0;   // 링 버퍼 시작 인덱스
static uint16_t gHistCount = 0;  // 실제 저장된 수

static void historyPush(float temp, float humidity, float pressure, float altitude, uint32_t ms) {
  uint16_t idx = (gHistHead + gHistCount) % HISTORY_SIZE;
  gHistory[idx] = { temp, humidity, pressure, altitude, ms };
  if (gHistCount < HISTORY_SIZE) {
    gHistCount++;
  } else {
    gHistHead = (gHistHead + 1) % HISTORY_SIZE;  // 가장 오래된 것 덮어쓰기
  }
}

static String historyJson() {
  String s = "{\"temp\":[";
  for (uint16_t i = 0; i < gHistCount; i++) {
    uint16_t idx = (gHistHead + i) % HISTORY_SIZE;
    if (i) s += ',';
    s += String(gHistory[idx].temp, 2);
  }
  s += "],\"humidity\":[";
  for (uint16_t i = 0; i < gHistCount; i++) {
    uint16_t idx = (gHistHead + i) % HISTORY_SIZE;
    if (i) s += ',';
    s += String(gHistory[idx].humidity, 2);
  }
  s += "],\"pressure\":[";
  for (uint16_t i = 0; i < gHistCount; i++) {
    uint16_t idx = (gHistHead + i) % HISTORY_SIZE;
    if (i) s += ',';
    s += String(gHistory[idx].pressure, 2);
  }
  s += "],\"altitude\":[";
  for (uint16_t i = 0; i < gHistCount; i++) {
    uint16_t idx = (gHistHead + i) % HISTORY_SIZE;
    if (i) s += ',';
    s += String(gHistory[idx].altitude, 2);
  }
  s += "],\"ms\":[";
  for (uint16_t i = 0; i < gHistCount; i++) {
    uint16_t idx = (gHistHead + i) % HISTORY_SIZE;
    if (i) s += ',';
    s += String(gHistory[idx].ms);
  }
  s += "]}";
  return s;
}

static String htmlPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="ko">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>C3 Weather Station</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      background: #0f172a;
      color: #e5e7eb;
    }
    .wrap {
      max-width: 700px;
      margin: 0 auto;
      padding: 20px;
    }
    h1 {
      font-size: 24px;
      margin-bottom: 8px;
    }
    .sub {
      color: #94a3b8;
      margin-bottom: 20px;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
      gap: 14px;
    }
    .card {
      background: #1e293b;
      border-radius: 14px;
      padding: 16px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.25);
    }
    .label {
      font-size: 13px;
      color: #94a3b8;
      margin-bottom: 8px;
    }
    .value {
      font-size: 32px;
      font-weight: bold;
    }
    .unit {
      font-size: 12px;
      color: #94a3b8;
      margin-top: 4px;
    }
    .status {
      margin-top: 18px;
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
    }
    .badge {
      padding: 8px 12px;
      border-radius: 999px;
      font-size: 13px;
      font-weight: bold;
    }
    .ok  { background: #14532d; color: #bbf7d0; }
    .bad { background: #7f1d1d; color: #fecaca; }
    .dl  { background: #1e3a5f; color: #93c5fd; text-decoration: none; cursor: pointer; }
    .foot {
      margin-top: 20px;
      color: #94a3b8;
      font-size: 13px;
    }
    .charts {
      margin-top: 22px;
    }
    .chart-card {
      background: #1e293b;
      border-radius: 14px;
      padding: 16px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.25);
    }
    .chart-title {
      font-size: 16px;
      font-weight: bold;
      margin-bottom: 10px;
      color: #e5e7eb;
    }
    .chart-wrap {
      position: relative;
      width: 100%;
      height: 280px;
    }
    canvas {
      width: 100%;
      height: 100%;
      display: block;
      background: #0b1220;
      border-radius: 10px;
    }
    .legend {
      margin-top: 10px;
      display: flex;
      gap: 16px;
      flex-wrap: wrap;
      font-size: 13px;
      color: #cbd5e1;
    }
    .legend-item {
      display: flex;
      align-items: center;
      gap: 8px;
    }
    .legend-color {
      width: 14px;
      height: 14px;
      border-radius: 4px;
      display: inline-block;
    }
    @media (max-width: 480px) {
      .chart-wrap { height: 220px; }
    }
  </style>
</head>
<body>
  <div class="wrap">
    <h1>C3 Weather Station</h1>
    <div class="sub">ESP32-C3 SuperMini + BME280</div>

    <div class="grid">
      <div class="card">
        <div class="label">Temperature</div>
        <div class="value" id="temp">-</div>
        <div class="unit">°C</div>
      </div>
      <div class="card">
        <div class="label">Humidity</div>
        <div class="value" id="humidity">-</div>
        <div class="unit">%RH</div>
      </div>
      <div class="card">
        <div class="label">Pressure</div>
        <div class="value" id="pressure">-</div>
        <div class="unit">hPa</div>
      </div>
      <div class="card">
        <div class="label">Altitude</div>
        <div class="value" id="altitude">-</div>
        <div class="unit">m</div>
      </div>
    </div>

    <div class="status">
      <div class="badge" id="bmeStatus">BME280: -</div>
      <div class="badge" id="countStatus">Samples: -</div>
      <a class="badge dl" href="/api/csv" download>⬇ Download CSV</a>
      <label class="badge dl" style="cursor:pointer;">
        ⬆ Upload CSV<input type="file" id="csvFile" accept=".csv" style="display:none;">
      </label>
      <select id="modeSelect" style="padding:8px 12px;border-radius:999px;font-size:13px;font-weight:bold;border:none;cursor:pointer;background:#14532d;color:#bbf7d0;">
        <option value="live">▶ Live Mode</option>
        <option value="csv">📂 CSV Mode</option>
      </select>
    </div>

    <div class="charts">
      <div class="chart-card">
        <div class="chart-title">Temperature / Humidity</div>
        <div class="chart-wrap"><canvas id="envCanvas"></canvas></div>
        <div class="legend">
          <div class="legend-item"><span class="legend-color" style="background:#ef4444;"></span>Temp (°C, ±1.0°C)</div>
          <div class="legend-item"><span class="legend-color" style="background:#38bdf8;"></span>Humidity (%RH, ±3.0%)</div>
        </div>
      </div>
      <div class="chart-card" style="margin-top:16px;">
        <div class="chart-title">Pressure / Altitude</div>
        <div class="chart-wrap"><canvas id="paCanvas"></canvas></div>
        <div class="legend">
          <div class="legend-item"><span class="legend-color" style="background:#a78bfa;"></span>Pressure (hPa, ±1.0hPa)</div>
          <div class="legend-item"><span class="legend-color" style="background:#34d399;"></span>Altitude* (m, ±8m)</div>
        </div>
        <div style="margin-top:8px; font-size:12px; color:#64748b;">* 기압으로부터 추정한 값 (Estimated from pressure, ref. 1013.25 hPa)</div>
      </div>
    </div>

    <div class="foot">
      <div>Auto refresh: 1 second</div>
      <div>History: up to 20 min (server-side, persists across reconnects)</div>
      <div id="uptime">Uptime: -</div>
    </div>
  </div>

  <script>
    const MAX_POINTS = 120;  // 화면 표시 최대 포인트 (서버에서 최근 N개만 가져옴)
    const labels = [];
    const tempSeries = [];
    const humSeries  = [];
    const presSeries = [];
    const altSeries  = [];

    let csvMode = false;
    let lastReadCount = -1;

    function safeNum(v) {
      const n = Number(v);
      return Number.isFinite(n) ? n : null;
    }

    function resizeCanvas(canvas) {
      const ratio = window.devicePixelRatio || 1;
      const rect = canvas.getBoundingClientRect();
      const w = Math.max(10, Math.floor(rect.width * ratio));
      const h = Math.max(10, Math.floor(rect.height * ratio));
      if (canvas.width !== w || canvas.height !== h) {
        canvas.width = w;
        canvas.height = h;
      }
      const ctx = canvas.getContext('2d');
      ctx.setTransform(ratio, 0, 0, ratio, 0, 0);
      return { ctx, width: rect.width, height: rect.height };
    }

    function seriesBounds(data, errorBand) {
      let min = Infinity, max = -Infinity;
      data.forEach(v => { if (Number.isFinite(v)) { if (v < min) min = v; if (v > max) max = v; } });
      if (!Number.isFinite(min)) { min = 0; max = 100; }
      if (min === max) { min -= 1; max += 1; }
      const dataPad = (max - min) * 0.10;
      min -= dataPad; max += dataPad;
      const desiredRatio = 0.33;
      const minSpan = (errorBand * 2) / desiredRatio;
      const span = max - min;
      if (span < minSpan) {
        const center = (max + min) * 0.5;
        min = center - minSpan * 0.5;
        max = center + minSpan * 0.5;
      }
      return { min, max };
    }

    function drawDualGrid(ctx, x, y, w, h, tBounds, hBounds, tColor, hColor, yTicks) {
      // 수평 그리드
      ctx.strokeStyle = 'rgba(255,255,255,0.06)';
      ctx.lineWidth = 1;
      for (let i = 0; i <= yTicks; i++) {
        const yy = y + (i * h / yTicks);
        ctx.beginPath(); ctx.moveTo(x, yy); ctx.lineTo(x + w, yy); ctx.stroke();
      }
      // 왼쪽 Y축 레이블 (온도, 빨강)
      ctx.fillStyle = tColor;
      ctx.font = '12px Arial';
      ctx.textAlign = 'right';
      ctx.textBaseline = 'middle';
      for (let i = 0; i <= yTicks; i++) {
        const yy = y + (i * h / yTicks);
        const val = tBounds.max - ((tBounds.max - tBounds.min) * i / yTicks);
        ctx.fillText(val.toFixed(1), x - 6, yy);
      }
      // 오른쪽 Y축 레이블 (습도, 파랑)
      ctx.fillStyle = hColor;
      ctx.textAlign = 'left';
      for (let i = 0; i <= yTicks; i++) {
        const yy = y + (i * h / yTicks);
        const val = hBounds.max - ((hBounds.max - hBounds.min) * i / yTicks);
        ctx.fillText(val.toFixed(1), x + w + 6, yy);
      }
    }

    function drawXAxisLabels(ctx, x, y, w, h, labelArr) {
      ctx.fillStyle = '#94a3b8';
      ctx.font = '11px Arial';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'top';
      const count = labelArr.length;
      if (count < 2) return;
      const marks = Math.min(6, count);
      for (let i = 0; i < marks; i++) {
        const idx = Math.round((count - 1) * i / (marks - 1));
        const xx = x + (idx * w / (count - 1));
        ctx.fillText(labelArr[idx], xx, y + h + 8);
      }
    }

    function drawBand(ctx, x, y, w, h, data, bounds, color, margin) {
      const valid = data.map(v => Number.isFinite(v) ? v : null);
      const n = valid.length;
      if (n < 2) return;
      const { min, max } = bounds;
      const toY = v => y + h - ((v - min) / (max - min) * h);
      const toX = i => (n === 1) ? (x + w / 2) : (x + (i * w / (n - 1)));
      ctx.beginPath();
      let first = true;
      for (let i = 0; i < n; i++) {
        const v = valid[i]; if (v === null) continue;
        first ? ctx.moveTo(toX(i), toY(v + margin)) : ctx.lineTo(toX(i), toY(v + margin));
        first = false;
      }
      for (let i = n - 1; i >= 0; i--) {
        const v = valid[i]; if (v === null) continue;
        ctx.lineTo(toX(i), toY(v - margin));
      }
      ctx.closePath();
      ctx.save();
      ctx.globalAlpha = 0.18;
      ctx.fillStyle = color;
      ctx.fill();
      ctx.restore();
    }

    function drawLine(ctx, x, y, w, h, data, bounds, color) {
      const valid = data.map(v => Number.isFinite(v) ? v : null);
      const n = valid.length;
      if (n < 1) return;
      const { min, max } = bounds;
      ctx.strokeStyle = color;
      ctx.lineWidth = 2;
      ctx.beginPath();
      let started = false;
      for (let i = 0; i < n; i++) {
        const v = valid[i];
        if (v === null) { started = false; continue; }
        const px = (n === 1) ? (x + w / 2) : (x + (i * w / (n - 1)));
        const py = y + h - ((v - min) / (max - min) * h);
        if (!started) { ctx.moveTo(px, py); started = true; }
        else { ctx.lineTo(px, py); }
      }
      ctx.stroke();
      ctx.fillStyle = color;
      for (let i = 0; i < n; i++) {
        const v = valid[i];
        if (v === null) continue;
        const px = (n === 1) ? (x + w / 2) : (x + (i * w / (n - 1)));
        const py = y + h - ((v - min) / (max - min) * h);
        ctx.beginPath();
        ctx.arc(px, py, 2.2, 0, Math.PI * 2);
        ctx.fill();
      }
    }

    function drawChart(canvasId, left, right) {
      const canvas = document.getElementById(canvasId);
      if (!canvas) return;
      const r = resizeCanvas(canvas);
      const ctx = r.ctx;
      const width = r.width;
      const height = r.height;
      ctx.clearRect(0, 0, width, height);
      ctx.fillStyle = '#0b1220';
      ctx.fillRect(0, 0, width, height);

      const lpad = 46, rpad = 46, top = 12, bottom = 34;
      const plotX = lpad, plotY = top;
      const plotW = width - lpad - rpad;
      const plotH = height - top - bottom;

      ctx.strokeStyle = 'rgba(255,255,255,0.08)';
      ctx.strokeRect(plotX, plotY, plotW, plotH);

      const lBounds = seriesBounds(left.data,  left.error);
      const rBounds = seriesBounds(right.data, right.error);

      drawDualGrid(ctx, plotX, plotY, plotW, plotH, lBounds, rBounds, left.color, right.color, 4);
      drawXAxisLabels(ctx, plotX, plotY, plotW, plotH, labels);

      ctx.save();
      ctx.beginPath();
      ctx.rect(plotX, plotY, plotW, plotH);
      ctx.clip();
      drawBand(ctx, plotX, plotY, plotW, plotH, left.data,  lBounds, left.color,  left.error);
      drawBand(ctx, plotX, plotY, plotW, plotH, right.data, rBounds, right.color, right.error);
      drawLine(ctx, plotX, plotY, plotW, plotH, left.data,  lBounds, left.color);
      drawLine(ctx, plotX, plotY, plotW, plotH, right.data, rBounds, right.color);
      ctx.restore();
    }

    function setText(id, value, digits) {
      const el = document.getElementById(id);
      if (!el) return;
      if (typeof digits === 'number') {
        const n = Number(value);
        el.textContent = Number.isFinite(n) ? n.toFixed(digits) : '-';
      } else {
        el.textContent = value;
      }
    }

    async function loadData() {
      try {
        // 현재값 + 히스토리 병렬 요청
        const [resD, resH] = await Promise.all([
          fetch('/api/data',    { cache: 'no-store' }),
          fetch('/api/history', { cache: 'no-store' })
        ]);
        const d = await resD.json();
        const h = await resH.json();

        setText('temp',     d.temp,     1);
        setText('humidity', d.humidity, 1);
        setText('pressure', d.pressure, 1);
        setText('altitude', d.altitude, 1);

        const bme = document.getElementById('bmeStatus');
        if (bme) {
          bme.textContent = 'BME280: ' + (d.bmeOk ? 'OK' : 'ERROR');
          bme.className = 'badge ' + (d.bmeOk ? 'ok' : 'bad');
        }
        const cnt = document.getElementById('countStatus');
        if (cnt) cnt.textContent = 'Samples: ' + (d.readCount || 0);

        // 히스토리로 시리즈 교체 (최근 MAX_POINTS개)
        const n = h.temp.length;
        if (n === 0) return;

        lastReadCount = d.readCount || 0;

        if (csvMode) {
          return;
        }

        const start = Math.max(0, n - MAX_POINTS);
        tempSeries.length = 0;
        humSeries.length  = 0;
        presSeries.length = 0;
        altSeries.length  = 0;
        labels.length = 0;
        for (let i = start; i < n; i++) {
          tempSeries.push(safeNum(h.temp[i]));
          humSeries.push(safeNum(h.humidity[i]));
          presSeries.push(safeNum(h.pressure[i]));
          altSeries.push(safeNum(h.altitude[i]));
          const ms = h.ms[i];
          const hh = Math.floor(ms / 3600000);
          const mm = Math.floor((ms % 3600000) / 60000);
          const ss = Math.floor((ms % 60000) / 1000);
          labels.push(String(hh).padStart(2,'0') + ':' +
                      String(mm).padStart(2,'0') + ':' +
                      String(ss).padStart(2,'0'));
        }

        drawChart('envCanvas',
          { data: tempSeries, error: 1.0, color: '#ef4444' },
          { data: humSeries,  error: 3.0, color: '#38bdf8' });
        drawChart('paCanvas',
          { data: presSeries, error: 1.0,  color: '#a78bfa' },
          { data: altSeries,  error: 8.0,  color: '#34d399' });

        const ms = d.lastUpdateMs || 0;
        const hh = Math.floor(ms / 3600000);
        const mm = Math.floor((ms % 3600000) / 60000);
        const ss = Math.floor((ms % 60000) / 1000);
        setText('uptime', 'Uptime: ' +
          String(hh).padStart(2,'0') + ':' +
          String(mm).padStart(2,'0') + ':' +
          String(ss).padStart(2,'0'));

      } catch (e) {
        console.log('loadData error:', e);
      }
    }

    window.addEventListener('resize', () => {
      drawChart('envCanvas',
        { data: tempSeries, error: 1.0, color: '#ef4444' },
        { data: humSeries,  error: 3.0, color: '#38bdf8' });
      drawChart('paCanvas',
        { data: presSeries, error: 1.0, color: '#a78bfa' },
        { data: altSeries,  error: 8.0, color: '#34d399' });
    });

    // ── Clear Graph ──────────────────────────────────────────
    function setPaused(paused) {
      csvMode = paused;
      const sel = document.getElementById('modeSelect');
      if (sel) {
        sel.value = paused ? 'csv' : 'live';
        sel.style.background = paused ? '#1e3a5f' : '#14532d';
        sel.style.color      = paused ? '#93c5fd' : '#bbf7d0';
      }
    }

    document.getElementById('modeSelect').addEventListener('change', function() {
      const paused = this.value === 'csv';
      setPaused(paused);
      if (!paused) {
        labels.length = 0;
        tempSeries.length = 0; humSeries.length  = 0;
        presSeries.length = 0; altSeries.length  = 0;
      }
    });

    // ── Upload CSV ───────────────────────────────────────────
    // 컬럼: uptime_ms, uptime, temp_C, humidity_pct, pressure_hPa, altitude_m
    document.getElementById('csvFile').addEventListener('change', function() {
      const file = this.files[0];
      if (!file) return;
      const reader = new FileReader();
      reader.onload = function(e) {
        setPaused(true);
        const lines = e.target.result.split('\n').filter(l => l.trim());
        if (lines.length < 2) return;

        // 헤더 파싱으로 컬럼 인덱스 동적 확인
        const header = lines[0].split(',').map(h => h.trim());
        const ci = {
          uptime:   header.indexOf('uptime'),
          temp:     header.indexOf('temp_C'),
          humidity: header.indexOf('humidity_pct'),
          pressure: header.indexOf('pressure_hPa'),
          altitude: header.indexOf('altitude_m'),
        };

        labels.length = 0;
        tempSeries.length = 0; humSeries.length  = 0;
        presSeries.length = 0; altSeries.length  = 0;

        for (let i = 1; i < lines.length; i++) {
          const col = lines[i].split(',');
          if (col.length < 3) continue;
          if (ci.uptime   >= 0) labels.push(col[ci.uptime].trim());
          if (ci.temp     >= 0) tempSeries.push(safeNum(col[ci.temp]));
          if (ci.humidity >= 0) humSeries.push(safeNum(col[ci.humidity]));
          if (ci.pressure >= 0) presSeries.push(safeNum(col[ci.pressure]));
          if (ci.altitude >= 0) altSeries.push(safeNum(col[ci.altitude]));
        }

        drawChart('envCanvas',
          { data: tempSeries, error: 1.0, color: '#ef4444' },
          { data: humSeries,  error: 3.0, color: '#38bdf8' });
        drawChart('paCanvas',
          { data: presSeries, error: 1.0, color: '#a78bfa' },
          { data: altSeries,  error: 8.0, color: '#34d399' });

        // 파일 input 초기화 (같은 파일 재업로드 가능하도록)
        this.value = '';
      };
      reader.readAsText(file);
    });

    document.addEventListener('DOMContentLoaded', function() {
      loadData();
      setInterval(loadData, 1000);
    });
  </script>
</body>
</html>
)rawliteral";

  return html;
}

static String jsonBool(bool v) { return v ? "true" : "false"; }

static String sensorJson() {
  String s = "{";
  s += "\"temp\":"       + String(gData.temp, 2)     + ",";
  s += "\"humidity\":"   + String(gData.humidity, 2) + ",";
  s += "\"pressure\":"   + String(gData.pressure, 2) + ",";
  s += "\"altitude\":"   + String(gData.altitude, 2) + ",";
  s += "\"bmeOk\":"      + jsonBool(gData.bmeOk)     + ",";
  s += "\"readCount\":"  + String(gData.readCount)   + ",";
  s += "\"lastUpdateMs\":" + String(gData.lastUpdateMs);
  s += "}";
  return s;
}

static String csvString() {
  String s = "uptime_ms,uptime,temp_C,humidity_pct,pressure_hPa,altitude_m\n";
  for (uint16_t i = 0; i < gHistCount; i++) {
    uint16_t idx = (gHistHead + i) % HISTORY_SIZE;
    uint32_t ms = gHistory[idx].ms;
    uint8_t hh = ms / 3600000;
    uint8_t mm = (ms % 3600000) / 60000;
    uint8_t ss = (ms % 60000) / 1000;
    char buf[12];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hh, mm, ss);
    s += String(ms) + "," + buf + "," +
         String(gHistory[idx].temp, 2) + "," +
         String(gHistory[idx].humidity, 2) + "," +
         String(gHistory[idx].pressure, 2) + "," +
         String(gHistory[idx].altitude, 2) + "\n";
  }
  return s;
}


void webUiBegin(const char* apSsid, const char* apPass) {
  WiFi.mode(WIFI_AP);

  bool ok;
  if (apPass && strlen(apPass) >= 8) {
    ok = WiFi.softAP(apSsid, apPass);
  } else {
    ok = WiFi.softAP(apSsid);
  }

  IPAddress ip = WiFi.softAPIP();
  gApIp = ip.toString();

  Serial.println("[WEB] SoftAP start");
  Serial.print("[WEB] AP IP: ");
  Serial.println(gApIp);
  if (!ok) Serial.println("[WEB] SoftAP start failed");

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html; charset=utf-8", htmlPage());
  });

  server.on("/api/csv", HTTP_GET, []() {
    server.sendHeader("Content-Disposition",
      "attachment; filename=\"weather_" + String(millis() / 1000) + "s.csv\"");
    server.send(200, "text/csv; charset=utf-8", csvString());
  });

  server.on("/api/history", HTTP_GET, []() {
    server.send(200, "application/json; charset=utf-8", historyJson());
  });

  server.on("/api/data", HTTP_GET, []() {
    server.send(200, "application/json; charset=utf-8", sensorJson());
  });

  server.onNotFound([]() {
    server.send(404, "text/plain; charset=utf-8", "Not found");
  });

  server.begin();
  Serial.println("[WEB] HTTP server started");
}

void webUiLoop() {
  server.handleClient();
}

void webUiUpdate(const WebSensorData& data) {
  gData = data;
  if (data.bmeOk) {
    historyPush(data.temp, data.humidity, data.pressure, data.altitude, data.lastUpdateMs);
  }
}

String webUiGetApIp() {
  return gApIp;
}
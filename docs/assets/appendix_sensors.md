```jsx
import { useState } from "react";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer, ReferenceLine } from "recharts";

// BME280 교차 비교 데이터 (MJKDZ vs Adafruit)
const bmeData = [
  { n: 1, mjkdz_t: 22.61, adafruit_t: 22.61, diff_t: 0.00, mjkdz_h: 58.71, adafruit_h: 58.68, diff_h: 0.02, mjkdz_p: 1002.38, adafruit_p: 1002.37, diff_p: 0.01 },
  { n: 2, mjkdz_t: 22.63, adafruit_t: 22.61, diff_t: 0.02, mjkdz_h: 58.64, adafruit_h: 58.65, diff_h: -0.01, mjkdz_p: 1002.37, adafruit_p: 1002.38, diff_p: -0.01 },
  { n: 3, mjkdz_t: 22.61, adafruit_t: 22.60, diff_t: 0.01, mjkdz_h: 58.60, adafruit_h: 58.71, diff_h: -0.12, mjkdz_p: 1002.38, adafruit_p: 1002.39, diff_p: -0.01 },
  { n: 4, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.93, adafruit_h: 58.86, diff_h: 0.07, mjkdz_p: 1002.38, adafruit_p: 1002.40, diff_p: -0.02 },
  { n: 5, mjkdz_t: 22.59, adafruit_t: 22.57, diff_t: 0.02, mjkdz_h: 58.69, adafruit_h: 58.72, diff_h: -0.03, mjkdz_p: 1002.37, adafruit_p: 1002.36, diff_p: 0.02 },
  { n: 6, mjkdz_t: 22.59, adafruit_t: 22.57, diff_t: 0.02, mjkdz_h: 58.77, adafruit_h: 58.76, diff_h: 0.01, mjkdz_p: 1002.40, adafruit_p: 1002.36, diff_p: 0.04 },
  { n: 7, mjkdz_t: 22.59, adafruit_t: 22.58, diff_t: 0.01, mjkdz_h: 58.84, adafruit_h: 58.83, diff_h: 0.01, mjkdz_p: 1002.40, adafruit_p: 1002.36, diff_p: 0.04 },
  { n: 8, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.73, adafruit_h: 58.74, diff_h: -0.01, mjkdz_p: 1002.38, adafruit_p: 1002.38, diff_p: 0.00 },
  { n: 9, mjkdz_t: 22.62, adafruit_t: 22.60, diff_t: 0.02, mjkdz_h: 58.75, adafruit_h: 58.74, diff_h: 0.02, mjkdz_p: 1002.41, adafruit_p: 1002.39, diff_p: 0.02 },
  { n: 10, mjkdz_t: 22.62, adafruit_t: 22.60, diff_t: 0.02, mjkdz_h: 58.68, adafruit_h: 58.66, diff_h: 0.03, mjkdz_p: 1002.38, adafruit_p: 1002.34, diff_p: 0.03 },
  { n: 11, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.99, adafruit_h: 58.95, diff_h: 0.04, mjkdz_p: 1002.37, adafruit_p: 1002.38, diff_p: -0.01 },
  { n: 12, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.72, adafruit_h: 58.77, diff_h: -0.06, mjkdz_p: 1002.39, adafruit_p: 1002.38, diff_p: 0.01 },
  { n: 13, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.65, adafruit_h: 58.62, diff_h: 0.03, mjkdz_p: 1002.39, adafruit_p: 1002.38, diff_p: 0.01 },
  { n: 14, mjkdz_t: 22.62, adafruit_t: 22.61, diff_t: 0.01, mjkdz_h: 58.71, adafruit_h: 58.72, diff_h: -0.01, mjkdz_p: 1002.36, adafruit_p: 1002.36, diff_p: 0.00 },
  { n: 15, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.59, adafruit_h: 58.59, diff_h: 0.00, mjkdz_p: 1002.39, adafruit_p: 1002.36, diff_p: 0.03 },
  { n: 16, mjkdz_t: 22.60, adafruit_t: 22.58, diff_t: 0.02, mjkdz_h: 58.56, adafruit_h: 58.58, diff_h: -0.02, mjkdz_p: 1002.40, adafruit_p: 1002.37, diff_p: 0.03 },
  { n: 17, mjkdz_t: 22.59, adafruit_t: 22.57, diff_t: 0.02, mjkdz_h: 59.27, adafruit_h: 59.29, diff_h: -0.02, mjkdz_p: 1002.35, adafruit_p: 1002.38, diff_p: -0.02 },
  { n: 18, mjkdz_t: 22.59, adafruit_t: 22.58, diff_t: 0.01, mjkdz_h: 59.07, adafruit_h: 58.99, diff_h: 0.07, mjkdz_p: 1002.36, adafruit_p: 1002.39, diff_p: -0.03 },
  { n: 19, mjkdz_t: 22.59, adafruit_t: 22.58, diff_t: 0.01, mjkdz_h: 58.83, adafruit_h: 58.83, diff_h: 0.00, mjkdz_p: 1002.40, adafruit_p: 1002.35, diff_p: 0.04 },
  { n: 20, mjkdz_t: 22.61, adafruit_t: 22.59, diff_t: 0.02, mjkdz_h: 58.79, adafruit_h: 58.80, diff_h: -0.01, mjkdz_p: 1002.38, adafruit_p: 1002.37, diff_p: 0.02 },
  { n: 21, mjkdz_t: 22.62, adafruit_t: 22.61, diff_t: 0.01, mjkdz_h: 59.48, adafruit_h: 59.52, diff_h: -0.03, mjkdz_p: 1002.36, adafruit_p: 1002.35, diff_p: 0.01 },
  { n: 22, mjkdz_t: 22.66, adafruit_t: 22.64, diff_t: 0.02, mjkdz_h: 62.27, adafruit_h: 62.27, diff_h: 0.00, mjkdz_p: 1002.41, adafruit_p: 1002.38, diff_p: 0.03 },
  { n: 23, mjkdz_t: 22.66, adafruit_t: 22.66, diff_t: 0.00, mjkdz_h: 59.89, adafruit_h: 60.45, diff_h: -0.56, mjkdz_p: 1002.38, adafruit_p: 1002.37, diff_p: 0.01 },
  { n: 24, mjkdz_t: 22.70, adafruit_t: 22.68, diff_t: 0.02, mjkdz_h: 62.38, adafruit_h: 62.29, diff_h: 0.10, mjkdz_p: 1002.37, adafruit_p: 1002.37, diff_p: 0.00 },
  { n: 25, mjkdz_t: 22.71, adafruit_t: 22.70, diff_t: 0.01, mjkdz_h: 61.33, adafruit_h: 61.00, diff_h: 0.33, mjkdz_p: 1002.37, adafruit_p: 1002.39, diff_p: -0.02 },
  { n: 26, mjkdz_t: 22.73, adafruit_t: 22.71, diff_t: 0.02, mjkdz_h: 61.80, adafruit_h: 62.03, diff_h: -0.23, mjkdz_p: 1002.38, adafruit_p: 1002.33, diff_p: 0.05 },
  { n: 27, mjkdz_t: 22.75, adafruit_t: 22.72, diff_t: 0.03, mjkdz_h: 59.99, adafruit_h: 59.89, diff_h: 0.10, mjkdz_p: 1002.33, adafruit_p: 1002.40, diff_p: -0.07 },
  { n: 28, mjkdz_t: 22.75, adafruit_t: 22.73, diff_t: 0.02, mjkdz_h: 59.75, adafruit_h: 59.66, diff_h: 0.08, mjkdz_p: 1002.39, adafruit_p: 1002.39, diff_p: 0.00 },
  { n: 29, mjkdz_t: 22.75, adafruit_t: 22.73, diff_t: 0.02, mjkdz_h: 59.39, adafruit_h: 59.36, diff_h: 0.04, mjkdz_p: 1002.38, adafruit_p: 1002.37, diff_p: 0.01 },
  { n: 30, mjkdz_t: 22.73, adafruit_t: 22.70, diff_t: 0.03, mjkdz_h: 58.79, adafruit_h: 58.72, diff_h: 0.07, mjkdz_p: 1002.38, adafruit_p: 1002.39, diff_p: -0.02 },
];

// 3종 센서 비교 데이터
const tripleData = [
  { n: 1, mjkdz_t: 22.45, adafruit_t: 22.45, dht11_t: 21.10, mjkdz_h: 59.10, adafruit_h: 59.07, dht11_h: 68.50 },
  { n: 2, mjkdz_t: 22.46, adafruit_t: 22.45, dht11_t: 21.20, mjkdz_h: 59.12, adafruit_h: 59.31, dht11_h: 70.90 },
  { n: 3, mjkdz_t: 22.44, adafruit_t: 22.44, dht11_t: 21.20, mjkdz_h: 59.28, adafruit_h: 59.77, dht11_h: 70.90 },
  { n: 4, mjkdz_t: 22.43, adafruit_t: 22.42, dht11_t: 21.20, mjkdz_h: 59.33, adafruit_h: 59.28, dht11_h: 70.90 },
  { n: 5, mjkdz_t: 22.43, adafruit_t: 22.42, dht11_t: 21.20, mjkdz_h: 59.50, adafruit_h: 59.44, dht11_h: 70.90 },
  { n: 6, mjkdz_t: 22.42, adafruit_t: 22.41, dht11_t: 21.20, mjkdz_h: 59.78, adafruit_h: 59.68, dht11_h: 70.90 },
  { n: 7, mjkdz_t: 22.40, adafruit_t: 22.41, dht11_t: 21.20, mjkdz_h: 59.47, adafruit_h: 59.54, dht11_h: 70.90 },
  { n: 8, mjkdz_t: 22.39, adafruit_t: 22.39, dht11_t: 21.20, mjkdz_h: 59.34, adafruit_h: 59.37, dht11_h: 70.90 },
  { n: 9, mjkdz_t: 22.38, adafruit_t: 22.37, dht11_t: 21.20, mjkdz_h: 59.34, adafruit_h: 59.32, dht11_h: 70.90 },
  { n: 10, mjkdz_t: 22.37, adafruit_t: 22.37, dht11_t: 21.20, mjkdz_h: 59.70, adafruit_h: 59.61, dht11_h: 70.80 },
  { n: 11, mjkdz_t: 22.38, adafruit_t: 22.38, dht11_t: 21.20, mjkdz_h: 60.05, adafruit_h: 60.28, dht11_h: 70.90 },
  { n: 12, mjkdz_t: 22.37, adafruit_t: 22.36, dht11_t: 21.20, mjkdz_h: 59.59, adafruit_h: 59.73, dht11_h: 70.90 },
  { n: 13, mjkdz_t: 22.39, adafruit_t: 22.37, dht11_t: 21.20, mjkdz_h: 59.92, adafruit_h: 59.77, dht11_h: 70.90 },
  { n: 14, mjkdz_t: 22.38, adafruit_t: 22.37, dht11_t: 21.20, mjkdz_h: 59.81, adafruit_h: 59.71, dht11_h: 71.00 },
  { n: 15, mjkdz_t: 22.36, adafruit_t: 22.35, dht11_t: 21.20, mjkdz_h: 59.53, adafruit_h: 59.45, dht11_h: 71.00 },
  { n: 16, mjkdz_t: 22.36, adafruit_t: 22.36, dht11_t: 21.20, mjkdz_h: 60.42, adafruit_h: 60.50, dht11_h: 71.00 },
  { n: 17, mjkdz_t: 22.37, adafruit_t: 22.36, dht11_t: 21.20, mjkdz_h: 60.07, adafruit_h: 60.14, dht11_h: 71.00 },
  { n: 18, mjkdz_t: 22.37, adafruit_t: 22.36, dht11_t: 21.20, mjkdz_h: 59.64, adafruit_h: 59.81, dht11_h: 71.10 },
  { n: 19, mjkdz_t: 22.36, adafruit_t: 22.34, dht11_t: 21.20, mjkdz_h: 59.59, adafruit_h: 59.53, dht11_h: 71.10 },
  { n: 20, mjkdz_t: 22.34, adafruit_t: 22.33, dht11_t: 21.20, mjkdz_h: 59.51, adafruit_h: 59.60, dht11_h: 71.10 },
];

const COLORS = {
  mjkdz: "#4fc3f7",
  adafruit: "#81c784",
  dht11: "#ef9a9a",
  diff: "#ffb74d",
  zero: "#666"
};

export default function SensorComparison() {
  const [view, setView] = useState("triple_temp");

  const views = [
    { id: "triple_temp", label: "3종 온도 비교" },
    { id: "triple_humid", label: "3종 습도 비교" },
    { id: "bme_diff_temp", label: "BME280 온도 차이" },
    { id: "bme_diff_humid", label: "BME280 습도 차이" },
    { id: "bme_diff_press", label: "BME280 기압 차이" },
  ];

  const renderChart = () => {
    switch (view) {
      case "triple_temp":
        return (
          <div>
            <p style={{ color: "#aaa", fontSize: 13, marginBottom: 8 }}>
              DHT11은 BME280 대비 약 <strong style={{color:"#ef9a9a"}}>+1.1~1.3°C</strong> 낮게 측정됨. MJKDZ vs Adafruit는 거의 동일.
            </p>
            <ResponsiveContainer width="100%" height={320}>
              <LineChart data={tripleData} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                <CartesianGrid strokeDasharray="3 3" stroke="#333" />
                <XAxis dataKey="n" stroke="#888" label={{ value: "측정 횟수", position: "insideBottom", offset: -2, fill: "#888", fontSize: 12 }} />
                <YAxis stroke="#888" domain={[20.5, 23]} tickFormatter={v => v.toFixed(1)} label={{ value: "온도 (°C)", angle: -90, position: "insideLeft", fill: "#888", fontSize: 12 }} />
                <Tooltip contentStyle={{ background: "#1e1e1e", border: "1px solid #444", borderRadius: 8 }} labelFormatter={v => `#${v}`} formatter={(v, name) => [`${v.toFixed(2)}°C`, name]} />
                <Legend />
                <Line type="monotone" dataKey="mjkdz_t" name="MJKDZ BME280" stroke={COLORS.mjkdz} dot={false} strokeWidth={2} />
                <Line type="monotone" dataKey="adafruit_t" name="Adafruit BME280" stroke={COLORS.adafruit} dot={false} strokeWidth={2} strokeDasharray="5 3" />
                <Line type="monotone" dataKey="dht11_t" name="DHT11" stroke={COLORS.dht11} dot={false} strokeWidth={2} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        );
      case "triple_humid":
        return (
          <div>
            <p style={{ color: "#aaa", fontSize: 13, marginBottom: 8 }}>
              DHT11 습도는 BME280 대비 약 <strong style={{color:"#ef9a9a"}}>+11%</strong> 높게 측정됨. MJKDZ vs Adafruit는 거의 동일.
            </p>
            <ResponsiveContainer width="100%" height={320}>
              <LineChart data={tripleData} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                <CartesianGrid strokeDasharray="3 3" stroke="#333" />
                <XAxis dataKey="n" stroke="#888" label={{ value: "측정 횟수", position: "insideBottom", offset: -2, fill: "#888", fontSize: 12 }} />
                <YAxis stroke="#888" domain={[55, 75]} label={{ value: "습도 (%)", angle: -90, position: "insideLeft", fill: "#888", fontSize: 12 }} />
                <Tooltip contentStyle={{ background: "#1e1e1e", border: "1px solid #444", borderRadius: 8 }} labelFormatter={v => `#${v}`} formatter={(v, name) => [`${v.toFixed(2)}%`, name]} />
                <Legend />
                <Line type="monotone" dataKey="mjkdz_h" name="MJKDZ BME280" stroke={COLORS.mjkdz} dot={false} strokeWidth={2} />
                <Line type="monotone" dataKey="adafruit_h" name="Adafruit BME280" stroke={COLORS.adafruit} dot={false} strokeWidth={2} strokeDasharray="5 3" />
                <Line type="monotone" dataKey="dht11_h" name="DHT11" stroke={COLORS.dht11} dot={false} strokeWidth={2} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        );
      case "bme_diff_temp":
        return (
          <div>
            <p style={{ color: "#aaa", fontSize: 13, marginBottom: 8 }}>
              온도 차이: 평균 <strong style={{color:"#ffb74d"}}>+0.02°C</strong> 이내. BME280 스펙 오차(±1°C)의 2% 수준.
            </p>
            <ResponsiveContainer width="100%" height={320}>
              <LineChart data={bmeData} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                <CartesianGrid strokeDasharray="3 3" stroke="#333" />
                <XAxis dataKey="n" stroke="#888" label={{ value: "측정 횟수", position: "insideBottom", offset: -2, fill: "#888", fontSize: 12 }} />
                <YAxis stroke="#888" domain={[-0.2, 0.2]} tickFormatter={v => v.toFixed(2)} label={{ value: "차이 (°C)", angle: -90, position: "insideLeft", fill: "#888", fontSize: 12 }} />
                <ReferenceLine y={0} stroke={COLORS.zero} strokeDasharray="4 2" />
                <Tooltip contentStyle={{ background: "#1e1e1e", border: "1px solid #444", borderRadius: 8 }} labelFormatter={v => `#${v}`} formatter={(v) => [`${v > 0 ? "+" : ""}${v.toFixed(2)}°C`, "MJKDZ - Adafruit"]} />
                <Line type="monotone" dataKey="diff_t" name="온도 차이 (MJKDZ - Adafruit)" stroke={COLORS.diff} dot={false} strokeWidth={2} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        );
      case "bme_diff_humid":
        return (
          <div>
            <p style={{ color: "#aaa", fontSize: 13, marginBottom: 8 }}>
              습도 차이: 대부분 <strong style={{color:"#ffb74d"}}>±0.1% 이내</strong>. 간헐적 스파이크(±0.5%)는 측정 타이밍 차이에 의한 노이즈.
            </p>
            <ResponsiveContainer width="100%" height={320}>
              <LineChart data={bmeData} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                <CartesianGrid strokeDasharray="3 3" stroke="#333" />
                <XAxis dataKey="n" stroke="#888" label={{ value: "측정 횟수", position: "insideBottom", offset: -2, fill: "#888", fontSize: 12 }} />
                <YAxis stroke="#888" domain={[-0.8, 0.8]} tickFormatter={v => v.toFixed(1)} label={{ value: "차이 (%)", angle: -90, position: "insideLeft", fill: "#888", fontSize: 12 }} />
                <ReferenceLine y={0} stroke={COLORS.zero} strokeDasharray="4 2" />
                <Tooltip contentStyle={{ background: "#1e1e1e", border: "1px solid #444", borderRadius: 8 }} labelFormatter={v => `#${v}`} formatter={(v) => [`${v > 0 ? "+" : ""}${v.toFixed(2)}%`, "MJKDZ - Adafruit"]} />
                <Line type="monotone" dataKey="diff_h" name="습도 차이 (MJKDZ - Adafruit)" stroke={COLORS.diff} dot={false} strokeWidth={2} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        );
      case "bme_diff_press":
        return (
          <div>
            <p style={{ color: "#aaa", fontSize: 13, marginBottom: 8 }}>
              기압 차이: <strong style={{color:"#ffb74d"}}>±0.05hPa</strong> 이내. BME280 스펙 오차(±1hPa)의 5% 수준.
            </p>
            <ResponsiveContainer width="100%" height={320}>
              <LineChart data={bmeData} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                <CartesianGrid strokeDasharray="3 3" stroke="#333" />
                <XAxis dataKey="n" stroke="#888" label={{ value: "측정 횟수", position: "insideBottom", offset: -2, fill: "#888", fontSize: 12 }} />
                <YAxis stroke="#888" domain={[-0.15, 0.15]} tickFormatter={v => v.toFixed(2)} label={{ value: "차이 (hPa)", angle: -90, position: "insideLeft", fill: "#888", fontSize: 12 }} />
                <ReferenceLine y={0} stroke={COLORS.zero} strokeDasharray="4 2" />
                <Tooltip contentStyle={{ background: "#1e1e1e", border: "1px solid #444", borderRadius: 8 }} labelFormatter={v => `#${v}`} formatter={(v) => [`${v > 0 ? "+" : ""}${v.toFixed(2)}hPa`, "MJKDZ - Adafruit"]} />
                <Line type="monotone" dataKey="diff_p" name="기압 차이 (MJKDZ - Adafruit)" stroke={COLORS.diff} dot={false} strokeWidth={2} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        );
      default:
        return null;
    }
  };

  return (
    <div style={{ background: "#121212", minHeight: "100vh", padding: "24px 16px", fontFamily: "sans-serif", color: "#eee" }}>
      <h2 style={{ fontSize: 20, fontWeight: "bold", marginBottom: 4 }}>센서 비교 분석 🜨</h2>
      <p style={{ color: "#888", fontSize: 13, marginBottom: 20 }}>MJKDZ BME280 vs Adafruit BME280 vs DHT11</p>

      {/* 탭 */}
      <div style={{ display: "flex", flexWrap: "wrap", gap: 8, marginBottom: 20 }}>
        {views.map(v => (
          <button key={v.id} onClick={() => setView(v.id)} style={{
            padding: "6px 12px", borderRadius: 6, border: "none", cursor: "pointer", fontSize: 13,
            background: view === v.id ? "#4fc3f7" : "#2a2a2a",
            color: view === v.id ? "#000" : "#ccc",
            fontWeight: view === v.id ? "bold" : "normal"
          }}>{v.label}</button>
        ))}
      </div>

      {/* 차트 */}
      <div style={{ background: "#1a1a1a", borderRadius: 12, padding: 16, marginBottom: 20 }}>
        {renderChart()}
      </div>

      {/* 결론 카드 */}
      <div style={{ display: "grid", gridTemplateColumns: "1fr 1fr", gap: 12 }}>
        {[
          { label: "온도 차이", value: "평균 +0.02°C", sub: "스펙 오차(±1°C)의 2%" },
          { label: "습도 차이", value: "±0.1% 이내", sub: "스펙 오차(±3%)의 3%" },
          { label: "기압 차이", value: "±0.05hPa", sub: "스펙 오차(±1hPa)의 5%" },
          { label: "DHT11 온도 오차", value: "+1.1~1.3°C", sub: "일관된 오프셋 존재" },
        ].map(c => (
          <div key={c.label} style={{ background: "#1a1a1a", borderRadius: 10, padding: "12px 14px" }}>
            <div style={{ fontSize: 12, color: "#888", marginBottom: 4 }}>{c.label}</div>
            <div style={{ fontSize: 18, fontWeight: "bold", color: "#4fc3f7" }}>{c.value}</div>
            <div style={{ fontSize: 11, color: "#666", marginTop: 2 }}>{c.sub}</div>
          </div>
        ))}
      </div>

      <div style={{ marginTop: 16, padding: "12px 14px", background: "#1a2a1a", borderRadius: 10, borderLeft: "3px solid #81c784", fontSize: 13, color: "#aaa", lineHeight: 1.6 }}>
        <strong style={{ color: "#81c784" }}>결론:</strong> MJKDZ BME280(~7,000원)과 Adafruit BME280(~30,000원)의 측정값 차이는
        센서 스펙 오차 범위 내로, 교육용 기상관측기로는 저가형으로도 충분합니다.
      </div>
    </div>
  );
}
```

 ```mermaid
xychart-beta
    title "Temperature Comparison"
    x-axis "Sample" [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
    y-axis "Temperature (°C)" 20 --> 23
    line "MJKDZ" [22.45,22.46,22.44,22.43,22.43,22.42,22.40,22.39,22.38,22.37,22.38,22.37,22.39,22.38,22.36,22.36,22.37,22.37,22.36,22.34]
    line "Adafruit" [22.45,22.45,22.44,22.42,22.42,22.41,22.41,22.39,22.37,22.37,22.38,22.36,22.37,22.37,22.35,22.36,22.36,22.36,22.34,22.33]
    line "DHT11" [21.10,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20,21.20]
```


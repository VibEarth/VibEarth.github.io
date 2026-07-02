---

<style>
.chart-container{
    position: relative;
    width: 100%;
    height: clamp(380px, 55vw, 520px);
    margin: 30px 0;
}
</style>

## Sensor Comparison

### Temperature

<canvas id="temperatureChart"></canvas>

### Humidity

<canvas id="humidityChart"></canvas>

### Pressure

<canvas id="pressureChart"></canvas>

<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<script src="/assets/bme280/sensorCharts.js"></script>

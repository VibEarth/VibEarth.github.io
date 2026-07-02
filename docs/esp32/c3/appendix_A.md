
# ESP32 Project - C3 SuperMini - Appendix A. Sensors

---
<style>
.chart-container{
    position:relative;
    width:100%;
    height:420px;
    margin:2rem 0;
}

@media (max-width:768px){
    .chart-container{
        height:300px;
    }
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

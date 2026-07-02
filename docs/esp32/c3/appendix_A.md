
# ESP32 Project - C3 SuperMini - Appendix A. Sensors

---
<style>
.chart-container {  
position: relative;  
width: 100%;  
height: 350px;  
}  
  
@media (max-width: 768px) {  
	.chart-container {  
	height: 300px;  
	}  
}
</style>

## Sensor Comparison

### Temperature
<div class="chart-container">  
	<canvas id="temperatureChart"></canvas>  
</div>
### Humidity
  
<div class="chart-container">  
	<canvas id="humidityChart"></canvas>  
</div>
### Pressure
<div class="chart-container">  
	<canvas id="pressureChart"></canvas>  
</div>

<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<script src="/assets/bme280/sensorCharts.js"></script>

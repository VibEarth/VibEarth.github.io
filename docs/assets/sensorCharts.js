async function loadCSV(url) {
    const text = await fetch(url).then(r => r.text());

    const rows = text
    .trim()
    .replace(/\r/g, "")
    .split("\n")
    .map(r => r.split(","));

    const header = rows.shift();

    return rows.map(r => {
        let obj = {};
        header.forEach((h, i) => obj[h] = parseFloat(r[i]));
        return obj;
    });
}

function makeChart(id, title, labels, datasets, yTitle) {

    new Chart(document.getElementById(id), {

        type: 'line',

        data: {
            labels,
            datasets
        },

        options: {

            responsive: true,

            interaction:{
                intersect:false,
                mode:'index'
            },

            plugins:{
                title:{
                    display:true,
                    text:title
                },
                legend:{
                    position:'top'
                }
            },

            scales:{
                x:{
                    title:{
                        display:true,
                        text:'Sample'
                    }
                },
                y:{
                    title:{
                        display:true,
                        text:yTitle
                    }
                }
            }
        }

    });

}

(async ()=>{

const data = await loadCSV("/assets/sensor.csv");

const labels = data.map(d=>d.Index);

makeChart(

"temperatureChart",

"Temperature Comparison",

labels,

[
{
label:"MJKDZ BME280",
data:data.map(d=>d.MJKDZ_Temp),
borderColor:"#d62728",
backgroundColor:"#d62728",
pointRadius:0,
borderWidth:2
},
{
label:"Adafruit BME280",
data:data.map(d=>d.Adafruit_Temp),
borderColor:"#1f77b4",
backgroundColor:"#1f77b4",
pointRadius:0,
borderWidth:2
},
{
label:"DHT11",
data:data.map(d=>d.DHT11_Temp),
borderColor:"#2ca02c",
backgroundColor:"#2ca02c",
pointRadius:0,
borderWidth:2
}
],

"Temperature (°C)"

);

makeChart(

"humidityChart",

"Humidity Comparison",

labels,

[
{
label:"MJKDZ BME280",
data:data.map(d=>d.MJKDZ_Humid),
borderColor:"#d62728",
backgroundColor:"#d62728",
pointRadius:0,
borderWidth:2
},
{
label:"Adafruit BME280",
data:data.map(d=>d.Adafruit_Humid),
borderColor:"#1f77b4",
backgroundColor:"#1f77b4",
pointRadius:0,
borderWidth:2
},
{
label:"DHT11",
data:data.map(d=>d.DHT11_Humid),
borderColor:"#2ca02c",
backgroundColor:"#2ca02c",
pointRadius:0,
borderWidth:2
}
],

"Humidity (%)"

);

makeChart(

"pressureChart",

"Pressure Comparison",

labels,

[
{
label:"MJKDZ BME280",
data:data.map(d=>d.MJKDZ_Press),
borderColor:"#d62728",
backgroundColor:"#d62728",
pointRadius:0,
borderWidth:2
},
{
label:"Adafruit BME280",
data:data.map(d=>d.Adafruit_Press),
borderColor:"#1f77b4",
backgroundColor:"#1f77b4",
pointRadius:0,
borderWidth:2
}
],

"Pressure (hPa)"

);

})();

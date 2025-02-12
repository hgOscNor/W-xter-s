<template>
  <div style="flex-grow: 1">
    <canvas ref="lineChart"></canvas>
  </div>
</template>

<script setup>
import { ref, onMounted, watch } from 'vue'
import {
  Chart,
  LineController,
  LineElement,
  PointElement,
  LinearScale,
  Filler,
  Title,
  CategoryScale,
  scales,
  Tooltip,
  Legend,
} from 'chart.js'
import 'chartjs-adapter-date-fns'

// Props
const props = defineProps({
  name: String,
  lineColor: String,
  dataX: {
    type: Array,
  },
  dataY: {
    type: Array,
  },
  graphMin: Number,
  graphMax: Number,
})

// Register modules
Chart.register(
  LineController,
  LineElement,
  PointElement,
  LinearScale,
  Title,
  Filler,
  CategoryScale,
  scales,
  Tooltip,
  Legend,
)

const lineChart = ref(null)
let chartInstance = null

onMounted(() => {
  // Reset graph
  if (chartInstance) {
    chartInstance.destroy()
  }

  // Configure graph settings
  chartInstance = new Chart(lineChart.value, {
    type: 'line',
    // Set time on X-axis
    data: {
      labels: props.dataX.map((dateStr) => {
        const date = new Date(dateStr)
        date.setHours(date.getHours() - 1)
        return date
      }),
      datasets: [
        {
          data: props.dataY,
          fill: true,
          borderColor: props.lineColor,
          tension: 0.1,
        },
      ],
    },
    options: {
      maintainAspectRatio: false,
      pointRadius: 0,
      animation: false,
      scales: {
        x: {
          type: 'time',
          // Configure time format
          time: {
            unit: 'minute',
            tooltipFormat: 'yyyy-MM-dd HH:mm:',
            displayFormats: {
              minute: 'HH:mm',
              hour: 'HH:mm',
              day: 'MMM dd',
            },
          },
          ticks: {
            color: 'white', // Color of X-axis text
          },
        },
        y: {
          // Set min / max on Y axis
          suggestedMin: Number(props.graphMin),
          suggestedMax: Number(props.graphMax),
          ticks: {
            color: 'white', // Color of X-axis text
          },
        },
      },
      responsive: true,
      plugins: {
        legend: false,
        title: {
          display: true,
          text: props.name,
          color: 'white', // Färg på titeln
        },
        interaction: {
          mode: 'index',
          intersect: false,
        },
        tooltip: {
          enabled: true,
          mode: 'index',
          intersect: false,
        },
      },
    },
  })
})

// Updates graph when new data arrives
watch([() => props.dataX, () => props.dataY], () => {
  if (chartInstance) {
    // Update time
    chartInstance.data.labels = props.dataX.map((dateStr) => {
      const date = new Date(dateStr)
      date.setHours(date.getHours() - 1)
      return date
    })
    chartInstance.data.datasets[0].data = props.dataY
    chartInstance.update()
  }
})
</script>

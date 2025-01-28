<template>
  <div>
    <canvas ref="lineChart"></canvas>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import {
  Chart,
  LineController,
  LineElement,
  PointElement,
  LinearScale,
  Filler,
  Title,
  CategoryScale,
} from 'chart.js'

// Props
// const props = defineProps({
//   name: String,
// })

// Registrera moduler
Chart.register(LineController, LineElement, PointElement, LinearScale, Title, Filler, CategoryScale)

const lineChart = ref(null) // Referens till canvas-elementet

onMounted(() => {
  // Data och inställningar för linjediagrammet
  const data = {
    labels: [], // X-axelns etiketter
    datasets: [
      {
        label: 'My First Dataset',
        data: [65, 59, 80, 81, 56, 55, 40],
        fill: true,
        borderColor: 'rgb(75, 192, 192)',
        tension: 0.1,
      },
    ],
  }

  const config = {
    type: 'line', // Typ av diagram
    data: data,
    options: {
      responsive: true,
      plugins: {
        legend: {
          position: 'top',
        },
        title: {
          display: true,
          text: '',
        },
      },
    },
  }

  // Skapa diagrammet
  new Chart(lineChart.value, config)
})
</script>

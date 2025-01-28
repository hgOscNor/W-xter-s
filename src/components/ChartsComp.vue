<template>
  <div>
    <canvas ref="lineChart"></canvas>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { Chart } from 'chart.js'

const lineChart = ref(null) // Referens till canvas-elementet

onMounted(() => {
  // Data och inställningar för linjediagrammet
  const data = {
    labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'], // X-axelns etiketter
    datasets: [
      {
        label: 'My First Dataset',
        data: [65, 59, 80, 81, 56, 55, 40], // Datapunkter
        fill: true, // Ingen bakgrundsfyllning
        borderColor: 'rgb(75, 192, 192)', // Linjens färg
        tension: 0.1, // Gör linjen mjukt kurvig
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
          text: 'Line Chart Example',
        },
      },
    },
  }

  // Skapa diagrammet
  new Chart(lineChart.value, config)
})
</script>

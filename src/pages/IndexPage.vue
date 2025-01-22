<template>
  <q-btn :to="'/Manual'">Manual</q-btn>
  <div id="chart">
    <apexchart
      type="line"
      height="350"
      ref="chart"
      :options="chartOptions"
      :series="series"
    ></apexchart>
  </div>
</template>

<script setup>
//
import { Vue /*ref as vueRef,*/ /*onMounted,*/ /*watch*/ } from 'vue'

//Firebase
// import {} from /*useRouter*/ 'vue-router'
// import { useDatabaseObject } from 'vuefire'
// import { ref as dbRef, set } from 'firebase/database'
// import { db } from 'src/boot/vuefire'

//ApexCharts
import VueApexCharts from 'https://cdn.jsdelivr.net/npm/apexcharts'

var _seed = 42
Math.random = function () {
  _seed = (_seed * 16807) % 2147483647
  return (_seed - 1) / 2147483646
}

let data = 0

new Vue({
  el: '#app',
  components: {
    apexchart: VueApexCharts,
  },
  data: {
    series: [
      {
        data: data.slice(),
      },
    ],
    chartOptions: {
      chart: {
        id: 'realtime',
        height: 350,
        type: 'line',
        animations: {
          enabled: true,
          easing: 'linear',
          dynamicAnimation: {
            speed: 1000,
          },
        },
        toolbar: {
          show: false,
        },
        zoom: {
          enabled: false,
        },
      },
      dataLabels: {
        enabled: false,
      },
      stroke: {
        curve: 'smooth',
      },
      title: {
        text: 'Dynamic Updating Chart',
        align: 'left',
      },
      markers: {
        size: 0,
      },
      xaxis: {
        type: 'datetime',
        range: XAXISRANGE,
      },
      yaxis: {
        max: 100,
      },
      legend: {
        show: false,
      },
    },
  },
  mounted: function () {
    var me = this
    window.setInterval(function () {
      getNewSeries(lastDate, {
        min: 10,
        max: 90,
      })

      me.$refs.chart.updateSeries([
        {
          data: data,
        },
      ])
    }, 1000)

    // every 60 seconds, we reset the data to prevent memory leaks
    window.setInterval(function () {
      resetData()

      me.$refs.chart.updateSeries(
        [
          {
            data,
          },
        ],
        false,
        true,
      )
    }, 60000)
  },
})
</script>

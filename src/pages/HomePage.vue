<template>
  <q-btn :to="'/Manual'">Manual</q-btn>
  <ChartsComp
    name="Temperatur"
    lineColor="red"
    :dataX="tempDataX"
    :dataY="tempDataY"
    :graphMin="0"
    :graphMax="30"
  ></ChartsComp>
  <ChartsComp
    name="Humidtiy"
    lineColor="#fcba03"
    :dataX="humDataX"
    :dataY="humDataY"
    :graphMin="0"
    :graphMax="100"
  ></ChartsComp>
  <ChartsComp
    name="Earth"
    lineColor="green"
    :dataX="earthDataX"
    :dataY="earthDataY"
    :graphMin="0"
    :graphMax="100"
  ></ChartsComp>
  <ChartsComp
    name="Light"
    lineColor="blue"
    :dataX="lightDataX"
    :dataY="lightDataY"
    :graphMin="0"
    :graphMax="100"
  ></ChartsComp>
</template>
<script setup>
import ChartsComp from 'src/components/ChartsComp.vue'

//
import { ref as vueRef /*onMounted,*/, watch } from 'vue'

//Firebase
// import {} from /*useRouter*/ 'vue-router'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, query, limitToLast } from 'firebase/database'
import { db } from 'src/boot/vuefire'

const tempDataX = vueRef([])
const tempDataY = vueRef([])
const tempData = useDatabaseObject(query(dbRef(db, 'sensor/air/temp'), limitToLast(100)))

const humDataX = vueRef([])
const humDataY = vueRef([])
const humData = useDatabaseObject(query(dbRef(db, 'sensor/air/hum'), limitToLast(100)))

const earthDataX = vueRef([])
const earthDataY = vueRef([])
const earthData = useDatabaseObject(query(dbRef(db, 'sensor/earth'), limitToLast(100)))

const lightDataX = vueRef([])
const lightDataY = vueRef([])
const lightData = useDatabaseObject(query(dbRef(db, 'sensor/light'), limitToLast(100)))

watch(tempData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.C)
    tempDataX.value = timestamps
    tempDataY.value = values
  }
})
watch(humData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.moist)

    humDataX.value = timestamps
    humDataY.value = values
  }
})
watch(earthData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.soil)
    earthDataX.value = timestamps
    earthDataY.value = values
  }
})
watch(lightData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.light)
    lightDataX.value = timestamps
    lightDataY.value = values
  }
})
</script>

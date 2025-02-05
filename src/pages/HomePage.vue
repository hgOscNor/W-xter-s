<template>
  <q-page class="bg-green-10 full-height" >
    <q-carousel
      v-model="slide"
      swipeable
      animated
      control-color="primary"
      navigation
      padding
      arrows
      height="100%"
      class="bg-green-10"
    >
      <q-carousel-slide name="temp" class="row flex-center full-width full-height">
        <ChartsComp
          name="Temperatur"
          lineColor="red"
          :dataX="tempDataX"
          :dataY="tempDataY"
          :graphMin="0"
          :graphMax="30"
          style="width: 85%; height: 100%"
        />
      </q-carousel-slide>

      <q-carousel-slide name="humidity" class="row flex-center full-width full-height">
       <ChartsComp
         name="Humidity"
         lineColor="blue"
         :dataX="humDataX"
         :dataY="humDataY"
         :graphMin="0"
         :graphMax="100"
         style="width: 85%; height: 100%"
       />
      </q-carousel-slide>

      <q-carousel-slide name="earth" class="row flex-center full-width full-height">
        <ChartsComp
          name="Earth"
          lineColor="#6F4E37"
          :dataX="earthDataX"
          :dataY="earthDataY"
          :graphMin="0"
          :graphMax="100"
          style="width: 85%; height: 100%"
        />
      </q-carousel-slide>

      <q-carousel-slide name="light" class="row flex-center full-width full-height">
        <ChartsComp
          name="Light"
          lineColor="white"
          :dataX="lightDataX"
          :dataY="lightDataY"
          :graphMin="0"
          :graphMax="100"
          style="width: 85%; height: 100%"
        />
      </q-carousel-slide>
    </q-carousel>
  </q-page>

  <div class="">

  </div>
</template>

<script setup>
import { ref as vueRef, watch } from 'vue'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, query, limitToLast } from 'firebase/database'
import { db } from 'src/boot/vuefire'
import ChartsComp from 'src/components/ChartsComp.vue'



const slide = vueRef('temp') 

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

// Uppdatera grafer när Firebase-data ändras
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
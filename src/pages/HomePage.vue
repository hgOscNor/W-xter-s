<template>
  <q-page class="bg-green-10 full-height">
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
          style="width: 60%; height: 100%"
        />
      </q-carousel-slide>

      <q-carousel-slide name="humidity" class="row flex-center full-width full-height">
        <ChartsComp
          name="Luftfuktighet"
          lineColor="blue"
          :dataX="humDataX"
          :dataY="humDataY"
          :graphMin="0"
          :graphMax="100"
          style="width: 60%; height: 100%"
        />
      </q-carousel-slide>

      <q-carousel-slide name="earth" class="row flex-center full-width full-height">
        <ChartsComp
          name="Jord"
          lineColor="#6F4E37"
          :dataX="earthDataX"
          :dataY="earthDataY"
          :graphMin="0"
          :graphMax="100"
          style="width: 60%; height: 100%"
        />
      </q-carousel-slide>

      <q-carousel-slide name="light" class="row flex-center full-width full-height">
        <ChartsComp
          name="Ljus"
          lineColor="white"
          :dataX="lightDataX"
          :dataY="lightDataY"
          :graphMin="0"
          :graphMax="100"
          style="width: 60%; height: 100%"
        />
      </q-carousel-slide>
    </q-carousel>

    <div class="latestValuesContainer">
  <q-card v-for="(value, key) in latestValues" :key="key" class="latestValue">
    <q-card-section>
      <div class="text-h6">{{ key }}</div>
      <div class="text-h4">{{ value }}</div>
    </q-card-section>
  </q-card>
</div>

  </q-page>
</template>

<script setup>
import { ref as vueRef, watch } from 'vue'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, query, limitToLast } from 'firebase/database'
import { db } from 'src/boot/vuefire'
import ChartsComp from 'src/components/ChartsComp.vue'

const slide = vueRef('temp')

const latestValues = vueRef({
  Temperatur: '0°C',
  Luftfuktighet: '0%',
  Jordfuktighet: '0%',
  Ljusnivå: '0%'
})

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
    latestValues.value.Temperatur = `${values[values.length - 1] || 0}°C`
  }
})
watch(humData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.moist)
    humDataX.value = timestamps
    humDataY.value = values
    latestValues.value.Luftfuktighet = `${values[values.length - 1] || 0}%`
  }
})
watch(earthData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.soil)
    earthDataX.value = timestamps
    earthDataY.value = values
    latestValues.value.Jordfuktighet = `${values[values.length - 1] || 0}%`
  }
})
watch(lightData, (newdata) => {
  if (newdata) {
    const timestamps = Object.keys(newdata).sort()
    const values = timestamps.map((timestamp) => newdata[timestamp]?.light)
    lightDataX.value = timestamps
    lightDataY.value = values
    latestValues.value.Ljusnivå = `${values[values.length - 1] || 0}%`
  }
})
</script>

<style>
.latestValuesContainer {
  display: flex;
  justify-content: center;
  gap: 1.5%; 
  padding: 0.5%;
}

.latestValue {
  flex: 1; 
  max-width: 24%; 
  padding: 15px;
  border-radius: 40px;
  border: 2px solid black;
  text-align: center;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
  background-color: #4caf50;
}
</style>
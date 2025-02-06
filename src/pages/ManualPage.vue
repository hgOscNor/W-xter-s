<template> 
  <div class="controlPanel">
    <div class="controllers">
      <ControllerComp
        name="Fläkt kontroller"
        labelHead="Slå på vid:"
        labelTemp="°C"
        labelHum="Fuktighet"
        labelMain="Hastighet"
        labelManOve="Manuell överskrivning"
        labelManOn="Manuell på"
        :isOnValue="fanData?.isOn || false"
        :manualOverrideValue="fanData?.manualOverride || false"
        :inputValue="fanData?.manualSpeed || 0"
        :turnOnAtHumValue="fanData?.turnOnAtHum || 0"
        :turnOnAtTempValue="fanData?.turnOnAtTemp || 0"
        :manualOnValue="fan?.manualOn || false"
        @updateManualOverride="logManualOverrideFan"
        @updateManualSpeed="logManualSpeedFan"
        @updateTurnOnAtTemp="logTurnOnTempFan"
        @updateTurnOnAtHum="logTurnOnHumFan"
        @updateManualOn="logManualOnFan"
      ></ControllerComp>
    </div>

    <div class="controllers">
      <ControllerComp
        name="Pump kontroller"
        labelHead="Pumpa vid:"
        labelHum="Fuktighet"
        labelMain="Hastighet"
        labelManOve="Manuell överskrivning"
        labelManOn="Manuell på"
        :isOnValue="pumpData?.isOn || false"
        :manualOverrideValue="pumpData?.manualOverride || false"
        :inputValue="pumpData?.manualSpeed || 0"
        :turnOnAtHumValue="pumpData?.turnOnAtSoil || 0"
        :manualOnValue="pump?.manualOn || false"
        @updateManualOverride="logManualOverridePump"
        @updateManualSpeed="logManualSpeedPump"
        @updateTurnOnAtHum="logTurnOnSoilPump"
        @updateManualOn="logManualOnPump"
      ></ControllerComp>
    </div>

    <div class="controllers">
      <ControllerComp
        name="Lamp kontroller"
        labelHead="Tänd vid:"
        labelHum="Ljus värde"
        labelMain="Ljusstyrka"
        labelManOve="Manuell överskrivning"
        labelManOn="Manuell på"
        :isOnValue="lightsData?.isOn || false"
        :manualOverrideValue="lightsData?.manualOverride || false"
        :inputValue="lightsData?.dimmer || 0"
        :turnOnAtHumValue="lightsData?.turnOnAt || 0"
        :manualOnValue="lightsData?.manualOn || false"
        @updateManualOverride="logManualOverrideLights"
        @updateManualSpeed="logDimmer"
        @updateTurnOnAtHum="logTurnOnAtLights"
        @updateManualOn="logManualOnLights"
      ></ControllerComp>
    </div>

    <div class="controllers">
      <ControllerComp
        name="Luck kontroller"
        labelHead="Öppna vid:"
        labelTemp="°C"
        labelHum="Fuktighet"
        labelManOve="Manuell överskrivning"
        labelManOn="Manuell på"
        :isOnValue="trapdoorData?.isOn || false"
        :manualOverrideValue="trapdoorData?.manualOverride || false"
        :openValue="trapdoorData?.open || false"
        :turnOnAtHumValue="trapdoorData?.turnOnAtHum || 0"
        :turnOnAtTempValue="trapdoorData?.turnOnAtTemp || 0"
        :manualOnValue="trapdoorData?.manualOn || false"
        :inputValue="null"
        @updateManualOverride="logManualOverrideTrapdoor"
        @updateTurnOnAtTemp="logTurnOnTempTrapdoor"
        @updateTurnOnAtHum="logTurnOnHumTrapdoor"
        @updateManualOn="logManualOnTrapdoor"
      ></ControllerComp>
    </div>
  </div>
</template>

<script setup>
import ControllerComp from '../components/ControllerComp.vue'
import { ref as vueRef, watch } from 'vue'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, set } from 'firebase/database'
import { db } from 'src/boot/vuefire'

// Firebase
//    Ref
const fanManualOverrideRef = dbRef(db, 'control/fan/manualOverride')
const fanSpeedRef = dbRef(db, 'control/fan/manualSpeed')
const fanTurnOnAtTempRef = dbRef(db, 'control/fan/turnOnAtTemp')
const fanTurnOnAtHumRef = dbRef(db, 'control/fan/turnOnAtHum')
const fanManualOnRef = dbRef(db, 'control/fan/manualOn')

const pumpManualOverrideRef = dbRef(db, 'control/pump/manualOverride')
const pumpSpeedRef = dbRef(db, 'control/pump/manualSpeed')
const pumpTurnOnAtSoilRef = dbRef(db, 'control/pump/turnOnAtSoil')
const pumpManualOnRef = dbRef(db, 'control/pump/manualOn')

const trapdoorManualOverrideRef = dbRef(db, 'control/trapdoor/manualOverride')
const trapdoorTurnOnAtTempRef = dbRef(db, 'control/trapdoor/openAtTemp')
const trapdoorTurnOnAtHumRef = dbRef(db, 'control/trapdoor/openAtHum')
const trapdoorManualOnRef = dbRef(db, 'control/trapdoor/manualOn')

const lightsManualOverrdideRef = dbRef(db, 'control/lights/manualOverride')
const lightsDimmerRef = dbRef(db, 'control/lights/dimmer')
const lightsTurnOnAtRef = dbRef(db, 'control/lights/turnOnAt')
const lightsManualOnRef = dbRef(db, 'control/lights/manualOn')

//  Write to firebase
//    Fan
function logManualOverrideFan(data) {
  set(fanManualOverrideRef, Boolean(data))
}
function logManualSpeedFan(data) {
  set(fanSpeedRef, Number(data))
}
function logTurnOnTempFan(data) {
  set(fanTurnOnAtTempRef, Number(data))
}
function logTurnOnHumFan(data) {
  set(fanTurnOnAtHumRef, Number(data))
}
function logManualOnFan(data) {
  set(fanManualOnRef, Boolean(data))
}
//    Pump
function logManualOverridePump(data) {
  set(pumpManualOverrideRef, Boolean(data))
}
function logManualSpeedPump(data) {
  set(pumpSpeedRef, Number(data))
}
function logTurnOnSoilPump(data) {
  set(pumpTurnOnAtSoilRef, Number(data))
}
function logManualOnPump(data) {
  set(pumpManualOnRef, Boolean(data))
}
//    Trapdoor
function logManualOverrideTrapdoor(data) {
  set(trapdoorManualOverrideRef, Boolean(data))
}
function logTurnOnTempTrapdoor(data) {
  set(trapdoorTurnOnAtTempRef, Number(data))
}
function logTurnOnHumTrapdoor(data) {
  set(trapdoorTurnOnAtHumRef, Number(data))
}
function logManualOnTrapdoor(data) {
  set(trapdoorManualOnRef, Boolean(data))
}
//  Lights
function logManualOverrideLights(data) {
  set(lightsManualOverrdideRef, Boolean(data))
}
function logDimmer(data) {
  set(lightsDimmerRef, Number(data))
}
function logTurnOnAtLights(data) {
  set(lightsTurnOnAtRef, Number(data))
}
function logManualOnLights(data) {
  set(lightsManualOnRef, Boolean(data))
}

const data = useDatabaseObject(dbRef(db, 'control'))
const fanData = vueRef()
const pumpData = vueRef()
const trapdoorData = vueRef()
const lightsData = vueRef()

watch(data, (newData) => {
  if (newData?.fan) fanData.value = { ...newData.fan }
  if (newData?.pump) pumpData.value = { ...newData.pump }
  if (newData?.trapdoor) trapdoorData.value = { ...newData.trapdoor }
  if (newData?.lights) lightsData.value = { ...newData.lights }
})
</script>

<style>
.controlPanel {
  display: flex;
  justify-content: space-around;
  align-items: flex-start;
  flex-wrap: wrap;
  gap: 0.1%;
  padding: 0.5%;
}

.controllers {
  width: 24%;
  height: 100%;
  padding: 0.8%;
  padding-top: 0%;
  border-radius: 40px;
  border: 2px solid black;
  text-align: center;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
  background-color: #4caf50;
}
</style>
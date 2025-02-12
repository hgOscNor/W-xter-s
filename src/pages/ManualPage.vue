<template>
  <div class="controlPanel">
    <div class="controllers">
      <ControllerComp
        name="Fläkt kontroller"
        labelHead="Slå på vid:"
        labelTemp="°C"
        labelHum="Fuktighet"
        labelMain="Hastighet %"
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
        labelMain="Hastighet %"
        labelManOve="Manuell överskrivning"
        labelManOn="Manuell på"
        labelHumCustom="Jordfuktighet %"
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
        labelMain="Ljusstyrka % "
        labelManOve="Manuell överskrivning"
        labelManOn="Manuell på"
        labelHumCustom="Ljusnivå %"
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
const refs = {
  fan: {
    manualOverride: dbRef(db, 'control/fan/manualOverride'),
    manualSpeed: dbRef(db, 'control/fan/manualSpeed'),
    turnOnAtTemp: dbRef(db, 'control/fan/turnOnAtTemp'),
    turnOnAtHum: dbRef(db, 'control/fan/turnOnAtHum'),
    manualOn: dbRef(db, 'control/fan/manualOn'),
  },
  pump: {
    manualOverride: dbRef(db, 'control/pump/manualOverride'),
    manualSpeed: dbRef(db, 'control/pump/manualSpeed'),
    turnOnAtSoil: dbRef(db, 'control/pump/turnOnAtSoil'),
    manualOn: dbRef(db, 'control/pump/manualOn'),
  },
  trapdoor: {
    manualOverride: dbRef(db, 'control/trapdoor/manualOverride'),
    turnOnAtTemp: dbRef(db, 'control/trapdoor/openAtTemp'),
    turnOnAtHum: dbRef(db, 'control/trapdoor/openAtHum'),
    manualOn: dbRef(db, 'control/trapdoor/manualOn'),
  },
  lights: {
    manualOverride: dbRef(db, 'control/lights/manualOverride'),
    dimmer: dbRef(db, 'control/lights/dimmer'),
    turnOnAt: dbRef(db, 'control/lights/turnOnAt'),
    manualOn: dbRef(db, 'control/lights/manualOn'),
  },
}

//  Write to firebase
//    Fan
function logManualOverrideFan(data) {
  set(refs.fan.manualOverride, Boolean(data))
}
function logManualSpeedFan(data) {
  set(refs.fan.manualSpeed, Number(data))
}
function logTurnOnTempFan(data) {
  set(refs.fan.turnOnAtTemp, Number(data))
}
function logTurnOnHumFan(data) {
  set(refs.fan.turnOnAtHum, Number(data))
}
function logManualOnFan(data) {
  set(refs.fan.manualOn, Boolean(data))
}
//    Pump
function logManualOverridePump(data) {
  set(refs.pump.manualOverride, Boolean(data))
}
function logManualSpeedPump(data) {
  set(refs.pump.manualSpeed, Number(data))
}
function logTurnOnSoilPump(data) {
  set(refs.pump.turnOnAtSoil, Number(data))
}
function logManualOnPump(data) {
  set(refs.pump.manualOn, Boolean(data))
}
//    Trapdoor
function logManualOverrideTrapdoor(data) {
  set(refs.trapdoor.manualOverride, Boolean(data))
}
function logTurnOnTempTrapdoor(data) {
  set(refs.trapdoor.turnOnAtTemp, Number(data))
}
function logTurnOnHumTrapdoor(data) {
  set(refs.trapdoor.turnOnAtHum, Number(data))
}
function logManualOnTrapdoor(data) {
  set(refs.trapdoor.manualOn, Boolean(data))
}
//  Lights
function logManualOverrideLights(data) {
  set(refs.lights.manualOverride, Boolean(data))
}
function logDimmer(data) {
  set(refs.lights.dimmer, Number(data))
}
function logTurnOnAtLights(data) {
  set(refs.lights.turnOnAt, Number(data))
}
function logManualOnLights(data) {
  set(refs.lights.manualOn, Boolean(data))
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

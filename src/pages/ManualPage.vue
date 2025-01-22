<template>
  <div class="controllerObj">
<<<<<<< HEAD
    <div class="controllerBox">
      <ControllerComp
        name="Fläktkontroller"
        :isOnValue="fanData?.isOn || false"
        :manualOverrideValue="fanData?.manualOverride || false"
        :inputValue="fanData?.manualSpeed || 0"
        :turnOnAtHumValue="fanData?.turnOnAtHum || 0"
        :turnOnAtTempValue="fanData?.turnOnAtTemp || 0"
        @updateManualOverride="logManualOverrideFan"
        @updateManualSpeed="logManualSpeedFan"
        @updateTurnOnAtTemp="logTurnOnTempFan"
        @updateTurnOnAtHum="logTurnOnHumFan"
      ></ControllerComp>
    </div>
    <div class="controllerBox">
      <ControllerComp name="Pumpkontroller"></ControllerComp>
    </div>
=======
    <ControllerComp
      name="Fläkt kontroller"
      :isOnValue="fanData?.isOn || false"
      :manualOverrideValue="fanData?.manualOverride || false"
      :inputValue="fanData?.manualSpeed || 0"
      :turnOnAtHumValue="fanData?.turnOnAtHum || 0"
      :turnOnAtTempValue="fanData?.turnOnAtTemp || 0"
      @updateManualOverride="logManualOverrideFan"
      @updateManualSpeed="logManualSpeedFan"
      @updateTurnOnAtTemp="logTurnOnTempFan"
      @updateTurnOnAtHum="logTurnOnHumFan"
    ></ControllerComp>
    <ControllerComp
      name="Pump kontroller"
      :isOnValue="pumpData?.isOn || false"
      :manualOverrideValue="pumpData?.manualOverride || false"
      :inputValue="pumpData?.manualSpeed || 0"
      :turnOnAtHumValue="pumpData?.turnOnAtSoil || 0"
      @updateManualOverride="logManualOverridePump"
      @updateManualSpeed="logManualSpeedPump"
      @updateTurnOnAtHum="logTurnOnSoilPump"
    ></ControllerComp>
    <ControllerComp
      name="Luck kontroller"
      :isOnValue="trapdoorData?.isOn || false"
      :manualOverrideValue="trapdoorData?.manualOverride || false"
      :openValue="trapdoorData?.open || false"
      :turnOnAtHumValue="trapdoorData?.turnOnAtHum || 0"
      :turnOnAtTempValue="trapdoorData?.turnOnAtTemp || 0"
      :inputValue="null"
      @updateManualOverride="logManualOverrideTrapdoor"
      @updateManualSpeed="logManualOpenTrapdoor"
      @updateTurnOnAtTemp="logTurnOnTempTrapdoor"
      @updateTurnOnAtHum="logTurnOnHumTrapdoor"
    ></ControllerComp>
    <ControllerComp
      name="Lamp kontroller"
      :isOnValue="lights?.isOn || false"
      :manualOverrideValue="lights?.manualOverride || false"
      :turnOnAtHumValue="lights?.turnOnAt || 0"
      @updateManualOverride="logManualOverrideLights"
      @updateManualSpeed="logManualOpenLights"
      @updateTurnOnAtTemp="logTurnOnTempLights"
    ></ControllerComp>
>>>>>>> 64f03a59973dac0868283b645aefcc805554414c
  </div>
</template>


<script setup>
import ControllerComp from '../components/ControllerComp.vue'
import { ref as vueRef, /*onMounted,*/ watch } from 'vue'

//Firebase
import {} from /*useRouter*/ 'vue-router'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, set } from 'firebase/database'
import { db } from 'src/boot/vuefire'
// const router = useRouter()

// Firebase
//    Ref
const fanManualOverrideRef = dbRef(db, 'control/fan/manualOverride')
const fanSpeedRef = dbRef(db, 'control/fan/manualSpeed')
const fanTurnOnAtTempRef = dbRef(db, 'control/fan/turnOnAtTemp')
const fanTurnOnAtHumRef = dbRef(db, 'control/fan/turnOnAtHum')

const pumpManualOverrideRef = dbRef(db, 'control/pump/manualOverride')
const pumpSpeedRef = dbRef(db, 'control/pump/manualSpeed')
const pumpTurnOnAtSoilRef = dbRef(db, 'control/pump/turnOnAtSoil')

const trapdoorManualOverrideRef = dbRef(db, 'control/trapdoor/manualOverride')
const trapdoorSpeedRef = dbRef(db, 'control/trapdoor/open')
const trapdoorTurnOnAtTempRef = dbRef(db, 'control/trapdoor/turnOnAtTemp')
const trapdoorTurnOnAtHumRef = dbRef(db, 'control/trapdoor/turnOnAtHum')

const lightsManual
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
//    Trapdoor
function logManualOverrideTrapdoor(data) {
  set(trapdoorManualOverrideRef, Boolean(data))
}
function logManualOpenTrapdoor(data) {
  set(trapdoorSpeedRef, Boolean(data))
}
function logTurnOnTempTrapdoor(data) {
  set(trapdoorTurnOnAtTempRef, Number(data))
}
function logTurnOnHumTrapdoor(data) {
  set(trapdoorTurnOnAtHumRef, Number(data))
}
//  Lights
function logManualOpenLights() {
  set()
}

const data = useDatabaseObject(dbRef(db, 'control'))
const fanData = vueRef()
const pumpData = vueRef()
const trapdoorData = vueRef()
const lightsData = vueRef()
watch(data, (newData) => {
  if (newData && newData.fan) {
    fanData.value = { ...newData.fan }
  }
  if (newData && newData.pump) {
    pumpData.value = { ...newData.pump }
  }
  if (newData && newData.trapdoor) {
    trapdoorData.value = { ...newData.trapdoor }
  }
  if (newData && newData.lights) {
    lightsData.value = { ...newData.lights }
  }
})
</script>

<style>
.controllerObj {
  display: flex;
  justify-content: center; 
  align-items: flex-start; 
  flex-direction: row; 
  gap: 15%; 
}

.controllerBox {
  margin-top: 0%;
  width: 30%;
  border: 2px solid black;
  padding: 1px;
  display: flex;
  justify-content: center;
}
</style>

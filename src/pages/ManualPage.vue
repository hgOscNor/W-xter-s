<template>
  <div class="controllerObj">
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
    <ControllerComp name="Pump"></ControllerComp>
  </div>
</template>

<script setup>
import ControllerComp from '../components/ControllerComp.vue'
import { /*ref as vueRef*/ /*onMounted,*/ watch } from 'vue'

//Firebase
import {} from /*useRouter*/ 'vue-router'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, set } from 'firebase/database'
import { db } from 'src/boot/vuefire'
// const router = useRouter()

// Ref
const fanManualOverrideRef = dbRef(db, 'control/fan/manualOverride')
const fanSpeedRef = dbRef(db, 'control/fan/manualSpeed')
const fanTurnOnAtTempRef = dbRef(db, 'control/fan/turnOnAtTemp')
const fanTurnOnAtHumRef = dbRef(db, 'control/fan/turnOnAtHum')

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

const data = useDatabaseObject(dbRef(db, 'control'))
const fanData = defineModel({ default: 0 })
watch(data, (newData) => {
  if (newData && newData.fan) {
    fanData.value = { ...newData.fan }
  }
})
</script>

<style>
.controllerObj {
  display: flex;
  justify-content: center;
  flex-direction: column;
  align-items: center;
  flex-wrap: nowrap;
}
</style>

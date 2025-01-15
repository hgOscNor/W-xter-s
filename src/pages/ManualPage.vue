<template>
  <div class="controllerObj">
    <ControllerComp
      name="Fläktkontroller"
      :isOn="fanData?.isOn || false"
      v-model:startupValBool="fanData.manualOverride"
      v-model:startupValInt="fanData.manualSpeed"
      v-model:turnOnAtHum="fanData.turnOnAtHum"
      v-model:turnOnAtTemp="fanData.turnOnAtTemp"
    ></ControllerComp>
    <ControllerComp name="Pump"></ControllerComp>
  </div>
</template>

<script setup>
import ControllerComp from './components/ControllerComp.vue'
import { ref as vueRef, onMounted } from 'vue'
// import './firebase.js'

//

// Firebase
import { initializeApp } from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-app.js'
import {
  getDatabase,
  ref,
  onValue,
} from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-database.js'

const firebaseConfig = {
  apiKey: 'AIzaSyC3sxf274LVPT_RTw-I5TS4N6c3YbaMql4',
  authDomain: 'wexteras-8d3c3.firebaseapp.com',
  databaseURL: 'https://wexteras-8d3c3-default-rtdb.europe-west1.firebasedatabase.app',
  projectId: 'wexteras-8d3c3',
  storageBucket: 'wexteras-8d3c3.firebasestorage.app',
  messagingSenderId: '896283578937',
  appId: '1:896283578937:web:cc315228718995ae7273e8',
}

const app = initializeApp(firebaseConfig)
const db = getDatabase(app)

const fanRef = ref(db, 'control/fan')
//const pumpRef = ref(db, 'control/pump')
const fanData = vueRef({})
onMounted(() => {
  // const pumpData = vueRef(null)

  onValue(fanRef, (snapshot) => {
    fanData.value = snapshot.val()
    console.log(fanData.value.manualSpeed)
    console.log(fanData.value.manualOverride)
  })
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

<template>
  <q-layout view="lHh Lpr lFf">
    <q-header elevated class="bg-green text-white">
      <div class="q-pa-md">
        <q-tabs v-model="tab" class="text-white" align="center" style="height: 55px">
          <q-tab
            name="Hem"
            icon="img:https://www.hitachigymnasiet.se/wp-content/uploads/2023/09/logoMark.svg"
            label="Hem"
            style="padding: 0px 20px; font-size: 12px"
          />
          <q-tab
            name="Manual"
            icon="alarm"
            label="Manual"
            style="padding: 0px 20px; font-size: 12px"
          />
          <q-tab
            name="Om oss"
            icon="movie"
            label="Om oss"
            style="padding: 0px 20px; font-size: 12px"
          />
        </q-tabs>
      </div>
    </q-header>

    <q-page-container>
      <router-view />
    </q-page-container>
  </q-layout>
</template>

<script setup>
import { ref as vueRef /*onMounted,*/ /*watch*/ } from 'vue'

const tab = vueRef()
</script>

<style>

.q-layout {
  position: relative;
  overflow: hidden; 
}

.q-layout::before {
  content: '';
  position: absolute;
  width: 100%;
  height: 100%;
  background-image: url('/Greenhouse_28._Victor_Greenhouse.jpg');
  background-size: cover; 
  background-position: center; 
  background-repeat: no-repeat; 
  filter: blur(0px); 
  z-index: -1; 
}
</style>
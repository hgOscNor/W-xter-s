<template>
  <link
    rel="stylesheet"
    href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css"
  />
  <q-layout view="lHh Lpr lFf">
    <q-header elevated class="bg-green text-white">
      <div class="q-pa-md">
        <q-tabs v-model="tab" class="text-white" align="center">
          <q-route-tab
            to="/"
            name="home"
            icon="img:src\assets\120px-Breezeicons-actions-22-office-chart-area-stacked.svg-3467501506.png"
            label="Hem"
            style="padding: 0px 20px; font-size: 12px"
          />
          <q-route-tab
            name="manual"
            icon="fa fa-cogs"
            label="Kontroller"
            style="padding: 0px 20px; font-size: 12px"
            to="/manual"
          />
          <q-route-tab
            name="aboutUs"
            icon="fa-solid fa-circle-info"
            label="Om oss"
            style="padding: 0px 20px; font-size: 12px"
            to="/aboutUs"
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
import { ref as vueRef } from 'vue'

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

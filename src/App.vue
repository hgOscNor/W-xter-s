<template>
  <div class="controllerObj">
    <ControllerComp name="Fläktkontroller"></ControllerComp>
    <ControllerComp name="Pump"></ControllerComp>
  </div>
</template>

<script setup>
import ControllerComp from './components/ControllerComp.vue'
//
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

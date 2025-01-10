<template>
  <div class="inputDiv">
    <h3>{{ props.name }}</h3>

    <input type="number" min="0" max="100" v-model="inputValue" />
    <div>
      <q-slider v-model="inputValue" :min="0" :max="100" step="1" />
    </div>

    <q-toggle size="xl" class="checkbox" v-model="checkbox" />
  </div>
</template>

<script setup>
import { ref, watch } from 'vue'

const props = defineProps({
  name: String,
  startupValInt: Number,
  startupValBool: Boolean,
})

const inputValue = ref(startupValInt)
let checkbox = ref(startupValBool)

watch(checkbox, (val) => {
  console.log(val)
})

watch(inputValue, (val) => {
  if (val < 0) {
    inputValue.value = 0
  } else if (val > 100) {
    inputValue.value = 100
  }
})
</script>

<style>
input[type='number'] {
  background-color: #dedede;
  margin: 5%;
  padding: 5%;
  border-radius: 2px;
  border-style: solid;
  size: 100%;
}

.checkbox {
  color: red;
  transition-duration: 0.2ms;
}

/* .inputDiv {
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  gap: 10%;
} */
</style>

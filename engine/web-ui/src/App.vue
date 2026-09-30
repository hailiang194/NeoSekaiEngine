<script setup lang="ts">
import { onMounted, useTemplateRef } from 'vue'
import loadingOverlay from './components/loading-overlay.vue'
import * as initModule from './wasm/game'
import { Module } from './wasm/Module'

const canvasElement = useTemplateRef('canvas')

const module = new Module()
module.statusRef.value = 'Downloading...'

onMounted(() => {
  module.canvas = canvasElement.value
  module.canvas?.addEventListener(
    'webglcontextlost',
    (e: Event) => {
      alert('WebGL context lost. You will need to reload the page.')
      e.preventDefault()
    },
    false,
  )
  module.canvas?.addEventListener('contextmenu', (e: Event) => {
    e.preventDefault()
  })
  initModule.default(module)
})

window.onerror = () => {
  module.setStatus('Exception thrown, see JavaScript console')
  module.loadPercentRef.value = 0.0
  module.setStatus = (text: string) => {
    if (text) module.printErr('[post-exception status] ' + text)
  }
}
</script>

<template>
  <loading-overlay
    v-if="module.loadPercentRef.value < 100"
    :status="module.statusRef.value"
    :percentage="module.loadPercentRef.value"
  />
  <div class="stage">
    <canvas id="canvas" ref="canvas" tabindex="-1"></canvas>
  </div>
</template>

<style scoped>
.stage {
  width: 100%;
  height: 100%;
  display: grid;
  place-items: center;
}
</style>
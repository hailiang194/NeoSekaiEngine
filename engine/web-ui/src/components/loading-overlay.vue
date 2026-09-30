<script setup lang="ts">
const props = defineProps({
  status: { type: String, required: true },
  percentage: { type: Number, required: true },
})
</script>

<template>
  <div class="overlay">
    <div class="panel">
      <p class="status">{{ props.status }}</p>
      <div class="bar">
        <div class="fill" :style="{ width: `${Math.max(props.percentage, 0.1)}%` }"></div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.overlay {
  position: fixed;
  inset: 0;
  z-index: 50;
  display: grid;
  place-items: center;
  background: #334155;
}

.status {
  color: #ecfeff;
  text-align: center;
}

.bar {
  width: 24rem;
  height: 1.25rem;
  border: 2px solid #94a3b8;
  border-radius: 0.5rem;
  overflow: hidden;
  background: #020617;
}

.fill {
  height: 100%;
  background: #ffffff;
}
</style>
<template>
   <v-sheet
      :class="drawerVisible ? 'view-wrapper drawer-visible' : 'view-wrapper'"
      color="background"
   >
      <slot></slot>
   </v-sheet>
</template>
<script setup lang="ts">
import { ref } from "vue";
import { storeToRefs } from "pinia";
import useAppStore from "@/store/app";
const { drawerVisible } = storeToRefs(useAppStore());
</script>
<style scoped>
.view-wrapper {
   width: 100%;
   min-height: 100%;
   padding: 0;
   margin: 0;
}

.slide-enter-active.view-wrapper {
   width: calc(100% - var(--v-layout-left));
}
.slide-leave-active.view-wrapper {
   width: calc(100% - var(--v-layout-left));
}
</style>

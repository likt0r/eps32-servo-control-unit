<template>
   <v-app id="inspire" :theme="theme">
      <default-bar />
      <default-drawer />
      <v-main>
         <user-alerts />
         <router-view v-slot="{ Component }">
            <transition name="slide" :class="drawerVisible ? 'drawer' : ''">
               <component :is="Component" />
            </transition>
         </router-view>
      </v-main>
   </v-app>
</template>
<script lang="ts" setup>
import { storeToRefs } from "pinia";

import DefaultBar from "./AppBar.vue";
import DefaultDrawer from "./AppDrawer.vue";
import UserAlerts from "@/components/UserAlerts.vue";

import useAppStore from "@/store/app";
const app = useAppStore();

const { theme, drawerVisible } = storeToRefs(app);
</script>
<style scoped>
.slide-enter-active,
.slide-leave-active {
   transition-property: right, left;
   transition-duration: 0.55s;
   transition-timing-function: ease-out;
}

.slide-enter-to {
   position: absolute;
   right: 0;
}

.slide-enter-from {
   position: absolute;
   right: calc(-100% + var(--v-layout-left));
}

.slide-leave-to {
   position: absolute;
   left: calc(-100% + 2 * var(--v-layout-left));
}

.slide-leave-from {
   position: absolute;
   left: var(--v-layout-left);
}
</style>

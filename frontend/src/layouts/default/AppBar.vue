<template>
   <v-app-bar density="compact" color="primary">
      <v-app-bar-nav-icon @click="toggleDrawer"></v-app-bar-nav-icon>
      <v-toolbar-title>Application</v-toolbar-title>
      <template v-slot:append>
         <v-btn
            :icon="isDarkMode ? mdiLightSwitchOff : mdiLightSwitch"
            @click="toggleDarkMode"
         ></v-btn>
      </template>
   </v-app-bar>
</template>

<script lang="ts" setup>
import { storeToRefs } from "pinia";
import useAppStore from "@/store/app";
import { mdiLightSwitch, mdiLightSwitchOff } from "@mdi/js";

const { isDarkMode } = storeToRefs(useAppStore());
const { toggleDrawer, toggleDarkMode } = useAppStore();
</script>

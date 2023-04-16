<template>
   <v-app id="inspire" :theme="theme">
      <default-bar />
      <default-drawer />
      <v-main>
         <user-alerts />
         <router-view />
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
const { theme } = storeToRefs(app);
</script>

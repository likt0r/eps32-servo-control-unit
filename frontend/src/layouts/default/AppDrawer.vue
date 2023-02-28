<template>
   <v-navigation-drawer v-model="drawerVisible">
      <!--  -->
      <side-navigation :nav-points="navigationPoints"></side-navigation>
   </v-navigation-drawer>
</template>

<script lang="ts" setup>
import { storeToRefs } from "pinia";
import {
   mdiHome,
   mdiInformation,
   mdiLightbulb,
   mdiMovieOpenEdit,
} from "@mdi/js";
import useAppStore from "@/store/app";
import SideNavigation from "@/components/SideNavigation.vue";

const { drawerVisible } = storeToRefs(useAppStore());
const navigationPoints = [
   {
      title: "Home",
      icon: mdiHome,
      to: "/",
   },
   {
      title: "About",
      icon: mdiInformation,
      to: "/about",
   },
   {
      title: "LEDs",
      icon: mdiLightbulb,
      to: "/leds",
   },
   {
      title: "Editor",
      icon: mdiMovieOpenEdit,
      to: "/editor",
   },
];
</script>

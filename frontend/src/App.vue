<script setup lang="ts">
import { RouterLink, RouterView } from "vue-router";
import { ref } from 'vue'
import SideNavigation from "./components/SideNavigation.vue";

const theme = ref('light')
const drawer = ref(false)
const navigationPoints = ref([{title: "default", icon: "home", to: "/"}])

function onClick () {
  theme.value = theme.value === 'light' ? 'dark' : 'light'
}

function onDrawerClick () {
  console.log('drawer', drawer.value)
  drawer.value = !drawer.value
}
</script>

<template>
  <v-app :theme="theme">
<v-app-bar color="primary">
        <template v-slot:prepend>
          <v-app-bar-nav-icon  @click="onDrawerClick"></v-app-bar-nav-icon>
        </template>
    
        <v-toolbar-title>Application</v-toolbar-title>
      <v-spacer></v-spacer>
      <v-icon
          end
          icon="mdi-cancel"
        ></v-icon>
      <v-btn
        :prepend-icon="theme === 'light' ? 'mdi-weather-sunny' : 'mdi-weather-night'"
        @click="onClick"
      >Toggle Theme</v-btn>
    </v-app-bar>
    <v-navigation-drawer v-model="drawer">
        <SideNavigation :nav-points="navigationPoints" />
        </v-navigation-drawer>
    <v-main>
        <RouterView />

    
    </v-main>
  </v-app>


</template>

<style scoped>

</style>

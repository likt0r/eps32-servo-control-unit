// Composables
import { createRouter, createWebHistory } from "vue-router";

const routes = [
   {
      path: "/",
      component: () => import("@/layouts/default/Default.vue"),
      children: [
         {
            path: "",
            name: "Home",
            // route level code-splitting
            // this generates a separate chunk (about.[hash].js) for this route
            // which is lazy-loaded when the route is visited.
            component: () =>
               import(/* webpackChunkName: "home" */ "@/views/Home.vue"),
         },
      ],
   },

   {
      path: "/leds",
      component: () => import("@/layouts/default/Default.vue"),

      children: [
         {
            path: "",
            name: "leds",
            // route level code-splitting
            // this generates a separate chunk (about.[hash].js) for this route
            // which is lazy-loaded when the route is visited.
            props: (to: any) => ({ data: to.meta.data }),
            component: () => import("@/views/Leds.vue"),
         },
      ],
   },
   {
      path: "/editor",
      component: () => import("@/layouts/default/Default.vue"),

      children: [
         {
            path: "",
            name: "editor",
            // route level code-splitting
            // this generates a separate chunk (about.[hash].js) for this route
            // which is lazy-loaded when the route is visited.
            props: (to: any) => ({ data: to.meta.data }),
            component: () => import("@/views/Editor.vue"),
         },
      ],
   },
   {
      path: "/settings",
      component: () => import("@/layouts/default/Default.vue"),

      children: [
         {
            path: "",
            name: "settings",
            // route level code-splitting
            // this generates a separate chunk (about.[hash].js) for this route
            // which is lazy-loaded when the route is visited.
            // props: (to: any) => ({ data: to.meta.data }),
            props: (to: any) => ({ data: to.meta.data }),
            component: () => import("@/views/settings/Index.vue"),
         },
      ],
   },
];

const router = createRouter({
   history: createWebHistory(process.env.BASE_URL),
   routes,
});

export default router;

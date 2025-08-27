import { createRouter, createWebHistory } from "vue-router"
import YandexAuth from "../views/YandexAuth.vue"
import YandexCallback from "../views/YandexCallback.vue"

const routes = [
  { path: "/", component: YandexAuth },
  { path: "/auth/yandex", component: YandexCallback }
]

export default createRouter({
  history: createWebHistory(),
  routes,
})

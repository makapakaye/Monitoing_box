import './assets/main.css'
//載入creatapp函式
import { createApp } from 'vue'


//載入跟組件
import App from './App.vue'
//import router from './router'
//建立 Vue App 物件
const app = createApp(App)


//掛載到HTML標籤底下
app.mount('#app')

import Vue from 'vue'
import axios from 'axios'

import App from './App'
import router from './router'
import store from './store'

import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
Vue.use(ElementUI);

//组件之间通讯
Vue.prototype.$bus = new Vue()

import echarts from 'echarts'
Vue.prototype.$echarts = echarts

import electron_log from 'electron-log'
Vue.prototype.$electron_log = electron_log
electron_log.transports.file.file = "system.log";

import addon from 'HY_9010_T'
Vue.prototype.$addon = addon();



//引入sqllite3
import dboper from './database/sql_lite_database.js'
Vue.prototype.$dboper = dboper


import i18n from './i18n/i18n'
if (!process.env.IS_WEB) Vue.use(require('vue-electron'))
Vue.http = Vue.prototype.$http = axios
Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  components: { App },
  router,
  store,
  i18n,
  template: '<App/>'
}).$mount('#app')

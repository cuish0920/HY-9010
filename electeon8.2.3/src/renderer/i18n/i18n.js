import Vue from 'vue'
import locale from 'element-ui/lib/locale'
import VueI18n from 'vue-i18n'
import messages from './langs/index'
Vue.use(VueI18n)
const i18n = new VueI18n({
    //locale:"cn",
    locale: localStorage.lang || 'cn',
    //locale: localStorage.lang || 'cn', // 默认语言，考虑刷新操作，存入localStorage
    messages
  })
locale.i18n((key, value) => i18n.t(key, value)) //重点：为了实现element插件的多语言切换
export default i18n

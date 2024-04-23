import{ag as e,o as t,M as n,U as o,ay as r,d as a,j as i,c as s,a as m,K as c,as as l}from"./@vue-9d7d117a.js";import{c as d,a as p}from"./vue-router-d0599d8d.js";import{d as u,c as h}from"./pinia-c89a5039.js";import{i as _,z as f}from"./element-plus-1abe4ec3.js";/* empty css                    */import{Q as g}from"./@element-plus-9c1020e5.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-a55f440f.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./dayjs-9ee440cb.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";!function(){const e=document.createElement("link").relList;if(!(e&&e.supports&&e.supports("modulepreload"))){for(const e of document.querySelectorAll('link[rel="modulepreload"]'))t(e);new MutationObserver((e=>{for(const n of e)if("childList"===n.type)for(const e of n.addedNodes)"LINK"===e.tagName&&"modulepreload"===e.rel&&t(e)})).observe(document,{childList:!0,subtree:!0})}function t(e){if(e.ep)return;e.ep=!0;const t=function(e){const t={};return e.integrity&&(t.integrity=e.integrity),e.referrerPolicy&&(t.referrerPolicy=e.referrerPolicy),"use-credentials"===e.crossOrigin?t.credentials="include":"anonymous"===e.crossOrigin?t.credentials="omit":t.credentials="same-origin",t}(e);fetch(e.href,t)}}();const E=(e,t)=>{const n=e.__vccOpts||e;for(const[o,r]of t)n[o]=r;return n};const b=E({},[["render",function(a,i){const s=e("router-view");return t(),n(r,null,[o(s)],1024)}]]),I={},v=function(e,t,n){if(!t||0===t.length)return e();const o=document.getElementsByTagName("link");return Promise.all(t.map((e=>{if((e=function(e){return"/"+e}(e))in I)return;I[e]=!0;const t=e.endsWith(".css"),r=t?'[rel="stylesheet"]':"";if(!!n)for(let n=o.length-1;n>=0;n--){const r=o[n];if(r.href===e&&(!t||"stylesheet"===r.rel))return}else if(document.querySelector(`link[href="${e}"]${r}`))return;const a=document.createElement("link");return a.rel=t?"stylesheet":"modulepreload",t||(a.as="script",a.crossOrigin=""),a.href=e,document.head.appendChild(a),t?new Promise(((t,n)=>{a.addEventListener("load",t),a.addEventListener("error",(()=>n(new Error(`Unable to preload CSS for ${e}`))))})):void 0}))).then((()=>e())).catch((e=>{const t=new Event("vite:preloadError",{cancelable:!0});if(t.payload=e,window.dispatchEvent(t),!t.defaultPrevented)throw e}))},y={key:"like_admin_",set(e,t,n){e=this.getKey(e);let o={expire:n?this.time()+n:"",value:t};"object"==typeof o&&(o=JSON.stringify(o));try{window.localStorage.setItem(e,o)}catch(r){return null}},get(e){e=this.getKey(e);try{const t=window.localStorage.getItem(e);if(!t)return null;const{value:n,expire:o}=JSON.parse(t);return o&&o<this.time()?(window.localStorage.removeItem(e),null):n}catch(t){return null}},time:()=>Math.round((new Date).getTime()/1e3),remove(e){e=this.getKey(e),window.localStorage.removeItem(e)},getKey(e){return this.key+e}},L="token";function j(){return y.get(L)}function O(){P().resetState(),y.remove(L),function(){N.removeRoute(k);const{routes:e}=P();e.forEach((e=>{const t=e.name;t&&N.hasRoute(t)&&N.removeRoute(t)}))}()}var R=(e=>(e.LOGIN="/login",e.ERROR_403="/403",e.ERROR_404="/:pathMatch(.*)*",e.INDEX="/",e))(R||{});const w=[{icon:"shouye",name:"首页",url:"/admin/dashboard",paths:"admin/dashboard",childList:[]},{icon:"pcs",name:"PCS变流器",url:"/admin/pcs",paths:"admin/pcs",childList:[]},{icon:"bms",name:"BMS电池管理",url:"/admin/bms",paths:"admin/bms",childList:[]},{icon:"cooling",name:"温控系统",url:"/admin/ac",paths:"admin/ac",childList:[]},{icon:"xiaofang",name:"消防系统",url:"/admin/fe",paths:"admin/fe",childList:[]},{icon:"user-light",name:"环控系统",url:"/admin/env",paths:"admin/env",childList:[]},{icon:"online",name:"通信系统",url:"/admin/comm",paths:"admin/comm",childList:[]},{icon:"monitor",name:"控制系统",url:"/admin/control",paths:"admin/control",childList:[]},{icon:"upgrade",name:"固件升级",url:"/admin/upgrade",paths:"admin/upgrade",childList:[]},{icon:"event",name:"事件",url:"/admin/event",paths:"admin/event",childList:[]}],P=u({id:"user",state:()=>({token:j()||"",userInfo:{},routes:[],breadcrumb:[],menu:[],perms:[]}),getters:{},actions:{resetState(){this.token="",this.userInfo={},this.breadcrumb=[],this.perms=[]},login(e){var t;this.token=e.token,t=e,y.set(L,t.token,86400)},logout:()=>new Promise(((e,t)=>{N.push(R.LOGIN),O()})),getUserInfo(){},getMenu(){return new Promise(((e,t)=>{const n=w;this.menu=n,this.routes=T(n),e(n)}))}}});var S=(e=>(e.CATALOGUE="M",e.MENU="C",e.BUTTON="A",e))(S||{});function A(e){return/^(https?:|mailto:|tel:)/.test(e)}function T(e,t=!0){return e.map((e=>{const n=function(e,t){const n={path:A(e.paths)?e.paths:t?`/${e.paths}`:e.paths,name:Symbol(e.paths),meta:{hidden:!e.isShow,keepAlive:!!e.isCache,title:e.menuName,perms:e.perms,query:e.params,icon:e.menuIcon,type:e.menuType,activeMenu:e.selected}};switch(e.menuType){case S.CATALOGUE:n.component=t?LAYOUT:RouterView,e.children||(n.component=RouterView);break;case S.MENU:n.component=loadRouteView(e.component)}return n}(e,t);return null!=e.children&&e.children&&e.children.length&&(n.children=T(e.children,!1)),n}))}const x=[{path:"/",name:"App",redirect:"/login",component:b,children:[]},{path:"/admin",name:"Admin",meta:{title:"后台管理"},component:()=>v((()=>import("./Index-ae517b93.js")),["static/js/Index-ae517b93.js","static/js/vue-router-d0599d8d.js","static/js/@vue-9d7d117a.js","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/Index-c5d0d88c.css","static/css/animate-cf768824.css"]),redirect:"/admin/dashboard",children:[{path:"dashboard",name:"首页",meta:{title:"首页",breadcrumb:["后台管理","首页"]},component:()=>v((()=>import("./Index-e7e77d84.js")),["static/js/Index-e7e77d84.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-476dfe03.css","static/css/animate-cf768824.css"])},{path:"pcs",name:"PCS变流器",meta:{title:"PCS变流器",breadcrumb:["后台管理","PCS变流器"]},component:()=>v((()=>import("./Index-a3a2f6f1.js")),["static/js/Index-a3a2f6f1.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/index-3bd673e3.js","static/js/utils-e10d97cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-8056bce9.css","static/css/animate-cf768824.css"])},{path:"bms",name:"BMS电池管理",meta:{title:"BMS电池管理",breadcrumb:["后台管理","BMS电池管理"]},component:()=>v((()=>import("./Index-f03267e7.js")),["static/js/Index-f03267e7.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/index-3bd673e3.js","static/js/utils-e10d97cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-ea772ac1.css","static/css/animate-cf768824.css"])},{path:"ac",name:"温控系统",meta:{title:"温控系统",breadcrumb:["后台管理","温控系统"]},component:()=>v((()=>import("./Index-fad14de9.js")),["static/js/Index-fad14de9.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/index-3bd673e3.js","static/js/utils-e10d97cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-d8aaa3d9.css","static/css/animate-cf768824.css"])},{path:"fe",name:"消防系统",meta:{title:"消防系统",breadcrumb:["后台管理","消防系统"]},component:()=>v((()=>import("./Index-47787b43.js")),["static/js/Index-47787b43.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/Index-11bc9680.css","static/css/animate-cf768824.css"])},{path:"env",name:"环控系统",meta:{title:"环控系统",breadcrumb:["后台管理","环控系统"]},component:()=>v((()=>import("./Index-7918975f.js")),["static/js/Index-7918975f.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/index-3bd673e3.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-156d0585.css","static/css/animate-cf768824.css"])},{path:"comm",name:"通信系统",meta:{title:"通信系统",breadcrumb:["后台管理","通信系统"]},component:()=>v((()=>import("./Index-a81e22c2.js")),["static/js/Index-a81e22c2.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/index-3bd673e3.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-a3341314.css","static/css/animate-cf768824.css"])},{path:"control",name:"控制系统",meta:{title:"控制系统",breadcrumb:["后台管理","控制系统"]},component:()=>v((()=>import("./Index-c64ff0dd.js")),["static/js/Index-c64ff0dd.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/Index-55468722.css","static/css/animate-cf768824.css"])},{path:"upgrade",name:"固件升级",meta:{title:"固件升级",breadcrumb:["后台管理","固件升级"]},component:()=>v((()=>import("./Index-8318330c.js")),["static/js/Index-8318330c.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/animate-cf768824.css"])},{path:"cooling",name:"液冷系统",meta:{title:"液冷系统",breadcrumb:["后台管理","液冷系统"]},component:()=>v((()=>import("./Index-fad14de9.js")),["static/js/Index-fad14de9.js","static/js/user-67ad0d91.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/index-3bd673e3.js","static/js/utils-e10d97cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-d8aaa3d9.css","static/css/animate-cf768824.css"])},{path:"event",name:"事件",meta:{title:"事件",breadcrumb:["后台管理","事件"]},component:()=>v((()=>import("./Index-10406e97.js")),["static/js/Index-10406e97.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/animate-cf768824.css"])}]},{path:"/:pathMatch(.*)*",name:"NotFound",meta:{title:"错误页面"},component:()=>v((()=>import("./NotFound-a5800e94.js")),["static/js/NotFound-a5800e94.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/animate-cf768824.css"])},{path:"/login",name:"Login",meta:{title:"登录"},component:()=>v((()=>import("./Index-51295b62.js")),["static/js/Index-51295b62.js","static/js/@vue-9d7d117a.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-3afb0b50.css","static/css/animate-cf768824.css"])}],k=Symbol();const N=d({history:p(),routes:x});R.LOGIN,R.INDEX;const D=[R.LOGIN,R.ERROR_403];N.beforeEach((async(e,t,n)=>{document.title=e.meta.title+" - 正辉能源";const o=P();if(await o.getMenu(),o.routes,D.includes(e.path))n();else if(o.token)try{await o.getMenu();o.routes;n()}catch(r){O()}n()})),N.afterEach((()=>{}));const V=["xlink:href"],M=E(a({__name:"svgIcon",props:{name:{type:String,required:!0},color:{type:String,default:""}},setup(e){const n=e,o=i((()=>`#icon-${n.name}`)),r=i((()=>n.name?`svg-icon icon-${n.name}`:"svg-icon"));return(n,a)=>(t(),s("svg",c({class:r.value},n.$attrs,{style:{color:e.color}}),[m("use",{"xlink:href":o.value},null,8,V)],16))}}),[["__scopeId","data-v-8de19606"]]),C=l(b),U=h();for(const[$,q]of Object.entries(g))C.component($,q);C.use(_,{locale:f,size:"small",zIndex:3e3}),C.use(N),C.use(U),C.component("SvgIcon",M),C.mount("#app");export{R as P,E as _,O as c,j as g,N as r,P as u};

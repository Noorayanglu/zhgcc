import{ag as e,o as t,M as n,U as o,ay as r,d as a,j as i,c as s,a as c,K as m,as as l}from"./@vue-9d7d117a.js";import{c as d,a as u}from"./vue-router-d0599d8d.js";import{N as p}from"./nprogress-867e68f1.js";import{d as h,c as _}from"./pinia-c89a5039.js";import{i as f,z as g}from"./element-plus-1abe4ec3.js";/* empty css                    */import{Q as E}from"./@element-plus-9c1020e5.js";import"./dayjs-9ee440cb.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-a55f440f.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";!function(){const e=document.createElement("link").relList;if(!(e&&e.supports&&e.supports("modulepreload"))){for(const e of document.querySelectorAll('link[rel="modulepreload"]'))t(e);new MutationObserver((e=>{for(const n of e)if("childList"===n.type)for(const e of n.addedNodes)"LINK"===e.tagName&&"modulepreload"===e.rel&&t(e)})).observe(document,{childList:!0,subtree:!0})}function t(e){if(e.ep)return;e.ep=!0;const t=function(e){const t={};return e.integrity&&(t.integrity=e.integrity),e.referrerPolicy&&(t.referrerPolicy=e.referrerPolicy),"use-credentials"===e.crossOrigin?t.credentials="include":"anonymous"===e.crossOrigin?t.credentials="omit":t.credentials="same-origin",t}(e);fetch(e.href,t)}}();const b=(e,t)=>{const n=e.__vccOpts||e;for(const[o,r]of t)n[o]=r;return n};const y=b({},[["render",function(a,i){const s=e("router-view");return t(),n(r,null,[o(s)],1024)}]]),I={},v=function(e,t,n){if(!t||0===t.length)return e();const o=document.getElementsByTagName("link");return Promise.all(t.map((e=>{if((e=function(e){return"/"+e}(e))in I)return;I[e]=!0;const t=e.endsWith(".css"),r=t?'[rel="stylesheet"]':"";if(!!n)for(let n=o.length-1;n>=0;n--){const r=o[n];if(r.href===e&&(!t||"stylesheet"===r.rel))return}else if(document.querySelector(`link[href="${e}"]${r}`))return;const a=document.createElement("link");return a.rel=t?"stylesheet":"modulepreload",t||(a.as="script",a.crossOrigin=""),a.href=e,document.head.appendChild(a),t?new Promise(((t,n)=>{a.addEventListener("load",t),a.addEventListener("error",(()=>n(new Error(`Unable to preload CSS for ${e}`))))})):void 0}))).then((()=>e())).catch((e=>{const t=new Event("vite:preloadError",{cancelable:!0});if(t.payload=e,window.dispatchEvent(t),!t.defaultPrevented)throw e}))},j={key:"like_admin_",set(e,t,n){e=this.getKey(e);let o={expire:n?this.time()+n:"",value:t};"object"==typeof o&&(o=JSON.stringify(o));try{window.localStorage.setItem(e,o)}catch(r){return null}},get(e){e=this.getKey(e);try{const t=window.localStorage.getItem(e);if(!t)return null;const{value:n,expire:o}=JSON.parse(t);return o&&o<this.time()?(window.localStorage.removeItem(e),null):n}catch(t){return null}},time:()=>Math.round((new Date).getTime()/1e3),remove(e){e=this.getKey(e),window.localStorage.removeItem(e)},getKey(e){return this.key+e}},L="token";function O(){return j.get(L)}function w(){S().resetState(),j.remove(L),function(){D.removeRoute(N);const{routes:e}=S();e.forEach((e=>{const t=e.name;t&&D.hasRoute(t)&&D.removeRoute(t)}))}()}var P=(e=>(e.LOGIN="/login",e.ERROR_403="/403",e.ERROR_404="/:pathMatch(.*)*",e.INDEX="/",e))(P||{});const R=[{icon:"shouye",name:"首页",url:"/admin/dashboard",paths:"admin/dashboard",childList:[]},{icon:"pcs",name:"PCS",url:"/admin/pcs",paths:"admin/pcs",childList:[]},{icon:"bms",name:"BMS",url:"/admin/bms",paths:"admin/bms",childList:[]},{icon:"cooling",name:"液冷系统",url:"/admin/cooling",paths:"admin/cooling",childList:[]},{icon:"user-light",name:"环控系统",url:"/admin/round",paths:"admin/round",childList:[]},{icon:"online",name:"通信接口",url:"/admin/interface",paths:"admin/interface",childList:[]},{icon:"monitor",name:"控制",url:"/admin/control",paths:"admin/control",childList:[]},{icon:"upgrade",name:"固件升级",url:"/admin/upgrade",paths:"admin/upgrade",childList:[]},{icon:"event",name:"事件",url:"/admin/event",paths:"admin/event",childList:[]}],S=h({id:"user",state:()=>({token:O()||"",userInfo:{},routes:[],breadcrumb:[],menu:[],perms:[]}),getters:{},actions:{resetState(){this.token="",this.userInfo={},this.breadcrumb=[],this.perms=[]},login(e){var t;this.token=e.token,t=e,j.set(L,t.token,86400)},logout:()=>new Promise(((e,t)=>{D.push(P.LOGIN),w()})),getUserInfo(){},getMenu(){return new Promise(((e,t)=>{const n=R;this.menu=n,this.routes=k(n),e(n)}))}}});var A=(e=>(e.CATALOGUE="M",e.MENU="C",e.BUTTON="A",e))(A||{});function T(e){return/^(https?:|mailto:|tel:)/.test(e)}function k(e,t=!0){return e.map((e=>{const n=function(e,t){const n={path:T(e.paths)?e.paths:t?`/${e.paths}`:e.paths,name:Symbol(e.paths),meta:{hidden:!e.isShow,keepAlive:!!e.isCache,title:e.menuName,perms:e.perms,query:e.params,icon:e.menuIcon,type:e.menuType,activeMenu:e.selected}};switch(e.menuType){case A.CATALOGUE:n.component=t?LAYOUT:RouterView,e.children||(n.component=RouterView);break;case A.MENU:n.component=loadRouteView(e.component)}return n}(e,t);return null!=e.children&&e.children&&e.children.length&&(n.children=k(e.children,!1)),n}))}const x=[{path:"/",name:"App",redirect:"/login",component:y,children:[]},{path:"/admin",name:"Admin",meta:{title:"后台管理"},component:()=>v((()=>import("./Index-949ba691.js")),["static/js/Index-949ba691.js","static/js/vue-router-d0599d8d.js","static/js/@vue-9d7d117a.js","static/js/nprogress-867e68f1.js","static/js/dayjs-9ee440cb.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/Index-b14cb957.css","static/css/animate-cf768824.css"]),redirect:"/admin/dashboard",children:[{path:"dashboard",name:"首页",meta:{title:"首页",breadcrumb:["后台管理","首页"]},component:()=>v((()=>import("./Index-ad99b5fa.js")),["static/js/Index-ad99b5fa.js","static/js/user-0e88d26a.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-db10c689.css","static/css/animate-cf768824.css"])},{path:"pcs",name:"PCS",meta:{title:"PCS",breadcrumb:["后台管理","PCS"]},component:()=>v((()=>import("./Index-78f507d2.js")),["static/js/Index-78f507d2.js","static/js/user-0e88d26a.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/utils-4226c3cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-4810fbdb.css","static/css/animate-cf768824.css"])},{path:"bms",name:"BMS",meta:{title:"BMS",breadcrumb:["后台管理","BMS"]},component:()=>v((()=>import("./Index-3b06707a.js")),["static/js/Index-3b06707a.js","static/js/user-0e88d26a.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/utils-4226c3cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-8cd03fc8.css","static/css/animate-cf768824.css"])},{path:"cooling",name:"液冷系统",meta:{title:"液冷系统",breadcrumb:["后台管理","液冷系统"]},component:()=>v((()=>import("./Index-021e9ff6.js")),["static/js/Index-021e9ff6.js","static/js/user-0e88d26a.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/utils-4226c3cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-b9f986bb.css","static/css/animate-cf768824.css"])},{path:"round",name:"环控系统",meta:{title:"环控系统",breadcrumb:["后台管理","环控系统"]},component:()=>v((()=>import("./Index-6156dd1a.js")),["static/js/Index-6156dd1a.js","static/js/user-0e88d26a.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/utils-4226c3cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-c5c4e190.css","static/css/animate-cf768824.css"])},{path:"interface",name:"通信接口",meta:{title:"通信接口",breadcrumb:["后台管理","通信接口"]},component:()=>v((()=>import("./Index-df27aab3.js")),["static/js/Index-df27aab3.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/nprogress-867e68f1.js","static/js/dayjs-9ee440cb.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/Index-0a7827e9.css","static/css/animate-cf768824.css"])},{path:"control",name:"控制",meta:{title:"控制",breadcrumb:["后台管理","控制"]},component:()=>v((()=>import("./Index-ecfdc7eb.js")),["static/js/Index-ecfdc7eb.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/nprogress-867e68f1.js","static/js/dayjs-9ee440cb.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/Index-55468722.css","static/css/animate-cf768824.css"])},{path:"upgrade",name:"固件升级",meta:{title:"固件升级",breadcrumb:["后台管理","固件升级"]},component:()=>v((()=>import("./Index-0bef45e4.js")),["static/js/Index-0bef45e4.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/nprogress-867e68f1.js","static/js/dayjs-9ee440cb.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/animate-cf768824.css"])},{path:"cooling",name:"液冷系统",meta:{title:"液冷系统",breadcrumb:["后台管理","液冷系统"]},component:()=>v((()=>import("./Index-021e9ff6.js")),["static/js/Index-021e9ff6.js","static/js/user-0e88d26a.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-efa01f16.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/@vue-9d7d117a.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/utils-4226c3cb.js","static/js/vue-router-d0599d8d.js","static/js/pinia-c89a5039.js","static/css/Index-b9f986bb.css","static/css/animate-cf768824.css"])},{path:"event",name:"事件",meta:{title:"事件",breadcrumb:["后台管理","事件"]},component:()=>v((()=>import("./Index-3f566723.js")),["static/js/Index-3f566723.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/nprogress-867e68f1.js","static/js/dayjs-9ee440cb.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/animate-cf768824.css"])}]},{path:"/:pathMatch(.*)*",name:"NotFound",meta:{title:"错误页面"},component:()=>v((()=>import("./NotFound-4506e73f.js")),["static/js/NotFound-4506e73f.js","static/js/@vue-9d7d117a.js","static/js/vue-router-d0599d8d.js","static/js/nprogress-867e68f1.js","static/js/dayjs-9ee440cb.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/css/animate-cf768824.css"])},{path:"/login",name:"Login",meta:{title:"登录"},component:()=>v((()=>import("./Index-557a7464.js")),["static/js/Index-557a7464.js","static/js/@vue-9d7d117a.js","static/js/feedback-179fee83.js","static/js/element-plus-1abe4ec3.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-a55f440f.js","static/js/@element-plus-9c1020e5.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-41f0a336.css","static/js/vue-router-d0599d8d.js","static/js/nprogress-867e68f1.js","static/css/nprogress-771398e6.css","static/js/pinia-c89a5039.js","static/css/Index-fca3cd2b.css","static/css/animate-cf768824.css"])}],N=Symbol();const D=d({history:u(),routes:x});p.configure({showSpinner:!1});const M=P.LOGIN;P.INDEX;const V=[P.LOGIN,P.ERROR_403];D.beforeEach((async(e,t,n)=>{p.start(),document.title=e.meta.title+" - 正辉能源";const o=S();if(await o.getMenu(),o.routes,V.includes(e.path))n();else if(o.token)try{await o.getMenu();o.routes;n()}catch(r){w(),n({path:M,query:{redirect:e.fullPath}})}else n({path:M,query:{redirect:e.fullPath}});p.done()})),D.afterEach((()=>{p.done()}));const C=["xlink:href"],U=b(a({__name:"svgIcon",props:{name:{type:String,required:!0},color:{type:String,default:""}},setup(e){const n=e,o=i((()=>`#icon-${n.name}`)),r=i((()=>n.name?`svg-icon icon-${n.name}`:"svg-icon"));return(n,a)=>(t(),s("svg",m({class:r.value},n.$attrs,{style:{color:e.color}}),[c("use",{"xlink:href":o.value},null,8,C)],16))}}),[["__scopeId","data-v-8de19606"]]),q=l(y),$=_();for(const[B,G]of Object.entries(E))q.component(B,G);q.use(f,{locale:g,size:"small",zIndex:3e3}),q.use(D),q.use($),q.component("SvgIcon",U),q.mount("#app");export{P,b as _,w as c,O as g,D as r,S as u};

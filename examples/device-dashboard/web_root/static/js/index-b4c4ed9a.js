function e(){import.meta.url,import("_").catch((()=>1))}import{ag as t,o as n,M as o,U as r,ay as a,d as i,j as s,c as m,a as c,K as l,as as p}from"./@vue-c5a747fc.js";import{c as d,a as u}from"./vue-router-641e505b.js";import{d as h,c as _}from"./pinia-801b3705.js";import{i as f}from"./element-plus-230cec57.js";/* empty css                    */import{Q as g}from"./@element-plus-388117bf.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./dayjs-9ee440cb.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";!function(){const e=document.createElement("link").relList;if(!(e&&e.supports&&e.supports("modulepreload"))){for(const e of document.querySelectorAll('link[rel="modulepreload"]'))t(e);new MutationObserver((e=>{for(const n of e)if("childList"===n.type)for(const e of n.addedNodes)"LINK"===e.tagName&&"modulepreload"===e.rel&&t(e)})).observe(document,{childList:!0,subtree:!0})}function t(e){if(e.ep)return;e.ep=!0;const t=function(e){const t={};return e.integrity&&(t.integrity=e.integrity),e.referrerPolicy&&(t.referrerPolicy=e.referrerPolicy),"use-credentials"===e.crossOrigin?t.credentials="include":"anonymous"===e.crossOrigin?t.credentials="omit":t.credentials="same-origin",t}(e);fetch(e.href,t)}}();const E=(e,t)=>{const n=e.__vccOpts||e;for(const[o,r]of t)n[o]=r;return n};const b=E({},[["render",function(e,i){const s=t("router-view");return n(),o(a,null,[r(s)],1024)}]]),I={},y=function(e,t,n){if(!t||0===t.length)return e();const o=document.getElementsByTagName("link");return Promise.all(t.map((e=>{if((e=function(e){return"/"+e}(e))in I)return;I[e]=!0;const t=e.endsWith(".css"),r=t?'[rel="stylesheet"]':"";if(!!n)for(let n=o.length-1;n>=0;n--){const r=o[n];if(r.href===e&&(!t||"stylesheet"===r.rel))return}else if(document.querySelector('link[href="'.concat(e,'"]').concat(r)))return;const a=document.createElement("link");return a.rel=t?"stylesheet":"modulepreload",t||(a.as="script",a.crossOrigin=""),a.href=e,document.head.appendChild(a),t?new Promise(((t,n)=>{a.addEventListener("load",t),a.addEventListener("error",(()=>n(new Error("Unable to preload CSS for ".concat(e)))))})):void 0}))).then((()=>e())).catch((e=>{const t=new Event("vite:preloadError",{cancelable:!0});if(t.payload=e,window.dispatchEvent(t),!t.defaultPrevented)throw e}))},L={key:"like_admin_",set(e,t,n){e=this.getKey(e);let o={expire:n?this.time()+n:"",value:t};"object"==typeof o&&(o=JSON.stringify(o));try{window.localStorage.setItem(e,o)}catch(r){return null}},get(e){e=this.getKey(e);try{const t=window.localStorage.getItem(e);if(!t)return null;const{value:n,expire:o}=JSON.parse(t);return o&&o<this.time()?(window.localStorage.removeItem(e),null):n}catch(t){return null}},time:()=>Math.round((new Date).getTime()/1e3),remove(e){e=this.getKey(e),window.localStorage.removeItem(e)},getKey(e){return this.key+e}},v="token";function j(){return L.get(v)}function O(){w().resetState(),L.remove(v),function(){D.removeRoute(k);const{routes:e}=w();e.forEach((e=>{const t=e.name;t&&D.hasRoute(t)&&D.removeRoute(t)}))}()}var P=(e=>(e.LOGIN="/login",e.ERROR_403="/403",e.ERROR_404="/:pathMatch(.*)*",e.INDEX="/",e))(P||{});const R=[{icon:"shouye",name:"首页",url:"/admin/dashboard",paths:"admin/dashboard",childList:[]},{icon:"monitor",name:"控制系统",url:"/admin/control",paths:"admin/control",childList:[]},{icon:"setup",name:"参数配置",url:"/admin/setup",paths:"admin/setup",childList:[]},{icon:"pcs",name:"PCS变流器",url:"/admin/pcs",paths:"admin/pcs",childList:[]},{icon:"bms",name:"BMS电池管理",url:"/admin/bms",paths:"admin/bms",childList:[]},{icon:"cooling",name:"温控系统",url:"/admin/ac",paths:"admin/ac",childList:[]},{icon:"xiaofang",name:"消防系统",url:"/admin/fe",paths:"admin/fe",childList:[]},{icon:"monitor",name:"计量系统",url:"/admin/metering",paths:"admin/metering",childList:[]},{icon:"user-light",name:"环控系统",url:"/admin/env",paths:"admin/env",childList:[]},{icon:"online",name:"通信系统",url:"/admin/comm",paths:"admin/comm",childList:[]},{icon:"upgrade",name:"固件升级",url:"/admin/upgrade",paths:"admin/upgrade",childList:[]},{icon:"event",name:"事件",url:"/admin/log",paths:"admin/log",childList:[]}],w=h({id:"user",state:()=>({token:j()||"",userInfo:{},routes:[],breadcrumb:[],menu:[],perms:[]}),getters:{},actions:{resetState(){this.token="",this.userInfo={},this.breadcrumb=[],this.perms=[]},login(e){var t;this.token=e.token,t=e,L.set(v,t.token,86400)},logout:()=>new Promise(((e,t)=>{D.push(P.LOGIN),O()})),getUserInfo(){},getMenu(){return new Promise(((e,t)=>{const n=R;this.menu=n,this.routes=T(n),e(n)}))}}});var A=(e=>(e.CATALOGUE="M",e.MENU="C",e.BUTTON="A",e))(A||{});function S(e){return/^(https?:|mailto:|tel:)/.test(e)}function T(e,t=!0){return e.map((e=>{const n=function(e,t){const n={path:S(e.paths)?e.paths:t?"/".concat(e.paths):e.paths,name:Symbol(e.paths),meta:{hidden:!e.isShow,keepAlive:!!e.isCache,title:e.menuName,perms:e.perms,query:e.params,icon:e.menuIcon,type:e.menuType,activeMenu:e.selected}};switch(e.menuType){case A.CATALOGUE:n.component=t?LAYOUT:RouterView,e.children||(n.component=RouterView);break;case A.MENU:n.component=loadRouteView(e.component)}return n}(e,t);return null!=e.children&&e.children&&e.children.length&&(n.children=T(e.children,!1)),n}))}const x=[{path:"/",name:"App",redirect:"/login",component:b,children:[]},{path:"/admin",name:"Admin",meta:{title:"后台管理"},component:()=>y((()=>import("./Index-bcfee82f.js")),["static/js/Index-bcfee82f.js","static/js/vue-router-641e505b.js","static/js/@vue-c5a747fc.js","static/js/pinia-801b3705.js","static/js/element-plus-230cec57.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/css/Index-5f782869.css","static/css/animate-cf768824.css"]),redirect:"/admin/dashboard",children:[{path:"dashboard",name:"首页",meta:{title:"首页",breadcrumb:["后台管理","首页"]},component:()=>y((()=>import("./Index-88d907ca.js")),["static/js/Index-88d907ca.js","static/js/echarts-84a2ba7b.js","static/js/zrender-1465eda9.js","static/js/@vue-c5a747fc.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/js/element-plus-230cec57.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/css/Index-1b04566b.css","static/css/animate-cf768824.css"])},{path:"pcs",name:"PCS变流器",meta:{title:"PCS变流器",breadcrumb:["后台管理","PCS变流器"]},component:()=>y((()=>import("./Index-f1cd1417.js")),["static/js/Index-f1cd1417.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/utils-e10d97cb.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-6b2b861b.css","static/css/animate-cf768824.css"])},{path:"bms",name:"BMS电池管理",meta:{title:"BMS电池管理",breadcrumb:["后台管理","BMS电池管理"]},component:()=>y((()=>import("./Index-759e40ea.js")),["static/js/Index-759e40ea.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/utils-e10d97cb.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-4133fde8.css","static/css/animate-cf768824.css"])},{path:"ac",name:"温控系统",meta:{title:"温控系统",breadcrumb:["后台管理","温控系统"]},component:()=>y((()=>import("./Index-93ea0db5.js")),["static/js/Index-93ea0db5.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/utils-e10d97cb.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-0836c9da.css","static/css/animate-cf768824.css"])},{path:"fe",name:"消防系统",meta:{title:"消防系统",breadcrumb:["后台管理","消防系统"]},component:()=>y((()=>import("./Index-a4d5a327.js")),["static/js/Index-a4d5a327.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-93cabee1.css","static/css/animate-cf768824.css"])},{path:"metering",name:"计量系统",meta:{title:"计量系统",breadcrumb:["后台管理","计量系统"]},component:()=>y((()=>import("./Index-147b2b26.js")),["static/js/Index-147b2b26.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-d2a2ed00.css","static/css/animate-cf768824.css"])},{path:"env",name:"环控系统",meta:{title:"环控系统",breadcrumb:["后台管理","环控系统"]},component:()=>y((()=>import("./Index-4349d0a2.js")),["static/js/Index-4349d0a2.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-d3d7aef2.css","static/css/animate-cf768824.css"])},{path:"comm",name:"通信系统",meta:{title:"通信系统",breadcrumb:["后台管理","通信系统"]},component:()=>y((()=>import("./Index-e15f3f34.js")),["static/js/Index-e15f3f34.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-1605a094.css","static/css/animate-cf768824.css"])},{path:"control",name:"控制系统",meta:{title:"控制系统",breadcrumb:["后台管理","控制系统"]},component:()=>y((()=>import("./Index-3a369088.js")),["static/js/Index-3a369088.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/utils-e10d97cb.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-d4eb9e1c.css","static/css/animate-cf768824.css"])},{path:"setup",name:"参数配置",meta:{title:"参数配置",breadcrumb:["后台管理","参数配置"]},component:()=>y((()=>import("./Index-050bd93c.js")),["static/js/Index-050bd93c.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/echarts-84a2ba7b.js","static/js/zrender-1465eda9.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-75c18be4.css","static/css/animate-cf768824.css"])},{path:"upgrade",name:"固件升级",meta:{title:"固件升级",breadcrumb:["后台管理","固件升级"]},component:()=>y((()=>import("./Index-8088107b.js")),["static/js/Index-8088107b.js","static/js/@vue-c5a747fc.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/js/element-plus-230cec57.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/css/animate-cf768824.css"])},{path:"cooling",name:"液冷系统",meta:{title:"液冷系统",breadcrumb:["后台管理","液冷系统"]},component:()=>y((()=>import("./Index-93ea0db5.js")),["static/js/Index-93ea0db5.js","static/js/user-1fde702d.js","static/js/index-c8621ebd.js","static/js/lodash-20cd73ca.js","static/js/dayjs-9ee440cb.js","static/js/axios-5b55c11d.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/@vue-c5a747fc.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/utils-e10d97cb.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-0836c9da.css","static/css/animate-cf768824.css"])},{path:"log",name:"事件",meta:{title:"事件",breadcrumb:["后台管理","事件"]},component:()=>y((()=>import("./Index-4258dd87.js")),["static/js/Index-4258dd87.js","static/js/@vue-c5a747fc.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/js/element-plus-230cec57.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/css/animate-cf768824.css"])}]},{path:"/:pathMatch(.*)*",name:"NotFound",meta:{title:"错误页面"},component:()=>y((()=>import("./NotFound-3a851545.js")),["static/js/NotFound-3a851545.js","static/js/@vue-c5a747fc.js","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/js/element-plus-230cec57.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/css/animate-cf768824.css"])},{path:"/login",name:"Login",meta:{title:"登录"},component:()=>y((()=>import("./Index-8f794a0c.js")),["static/js/Index-8f794a0c.js","static/js/@vue-c5a747fc.js","static/js/feedback-5363c0cc.js","static/js/element-plus-230cec57.js","static/js/lodash-es-fb3d0246.js","static/js/@vueuse-90035fc5.js","static/js/@element-plus-388117bf.js","static/js/@popperjs-b78c3215.js","static/js/@ctrl-91de2ec7.js","static/js/dayjs-9ee440cb.js","static/js/async-validator-cf877c1f.js","static/js/memoize-one-63ab667a.js","static/js/escape-html-92a447fa.js","static/js/normalize-wheel-es-3222b0a2.js","static/js/@floating-ui-9ca8b935.js","static/css/element-plus-8a0c7673.css","static/js/vue-router-641e505b.js","static/js/pinia-801b3705.js","static/css/Index-61693e02.css","static/css/animate-cf768824.css"])}],k=Symbol();const D=d({history:u(),routes:x}),V=P.LOGIN;P.INDEX;const N=[P.LOGIN,P.ERROR_403];D.beforeEach((async(e,t,n)=>{document.title=e.meta.title+" - 多棱多";const o=w();if(await o.getMenu(),o.routes,N.includes(e.path))n();else if(o.token)try{await o.getMenu();o.routes;n()}catch(r){O(),n({path:V,query:{redirect:e.fullPath}})}else n({path:V,query:{redirect:e.fullPath}})})),D.afterEach((()=>{}));const M=["xlink:href"],C=E(i({__name:"svgIcon",props:{name:{type:String,required:!0},color:{type:String,default:""}},setup(e){const t=e,o=s((()=>"#icon-".concat(t.name))),r=s((()=>t.name?"svg-icon icon-".concat(t.name):"svg-icon"));return(t,a)=>(n(),m("svg",l({class:r.value},t.$attrs,{style:{color:e.color}}),[c("use",{"xlink:href":o.value},null,8,M)],16))}}),[["__scopeId","data-v-8de19606"]]),U=p(b),q=_();for(const[B,G]of Object.entries(g))U.component(B,G);U.use(f,{size:"small",zIndex:3e3}),U.use(D),U.use(q),U.component("SvgIcon",C),U.mount("#app");export{P,E as _,e as __vite_legacy_guard,O as c,j as g,D as r,w as u};

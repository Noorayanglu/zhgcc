import{h as a}from"./user-38772cb0.js";import{j as e}from"./index-3bd673e3.js";import{d as s,e as l,i as t,Y as i,ag as n,c as r,U as m,O as u,o,F as d,a7 as p,M as v,a as c,R as j,T as f,az as g,aA as b}from"./@vue-9d7d117a.js";import{_ as x}from"./index-821eee1e.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-efa01f16.js";import"./feedback-179fee83.js";import"./element-plus-1abe4ec3.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-a55f440f.js";import"./@element-plus-9c1020e5.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./nprogress-867e68f1.js";import"./vue-router-d0599d8d.js";import"./pinia-c89a5039.js";/* empty css                    */const _=a=>(g("data-v-4d6368be"),a=a(),b(),a),h={class:"round"},k={class:"img-box img-mini-box"},w=["src"],y={class:"imgdata-box"},I=_((()=>c("div",{class:"imgdata-name"},"通信状态",-1))),z=_((()=>c("div",{class:"imgdata-name"},"数据更新时间",-1))),A={class:"imgdata-value"},D={class:"imgdata-value"},O=x(s({__name:"Index",setup(s){const g=l([]),b=l({}),x=l(null),_=l(2e3);t((()=>{B()})),i((()=>{O()}));const O=()=>{clearInterval(x.value),x.value=null},B=()=>{a().then((a=>{b.value=a,g.value=e(b.value),g.value.map((a=>(a.value=e(a.value),a))),g.value=C(g.value),O(),x.value=setInterval((()=>{B()}),_.value)})).catch((a=>{}))},C=a=>{let e=a;return e.map((a=>{switch(a.name){case"辅助电表":a.index=0;break;case"一氧化碳":a.index=1;break;case"除湿机":a.index=2;break;case"DIDO":a.index=3}})),e.sort(((a,e)=>a.index-e.index)),e};return(a,e)=>{const s=n("el-col"),l=n("el-row"),t=n("el-table-column"),i=n("el-table");return o(),r("div",h,[m(l,null,{default:u((()=>[(o(!0),r(d,null,p(g.value,(a=>(o(),v(s,{span:12,key:a.index},{default:u((()=>[m(l,null,{default:u((()=>[m(s,{span:12},{default:u((()=>[c("div",k,[c("img",{src:`/src/assets/images/proj/${"hk"+(a.index+1)}.png`,alt:""},null,8,w)]),c("div",y,[m(l,null,{default:u((()=>[b.value[a.name]["通信状态"]?(o(),v(s,{key:0,span:10},{default:u((()=>[I])),_:1})):j("",!0),m(s,{span:14},{default:u((()=>[z])),_:1}),m(s,{span:10},{default:u((()=>[c("div",A,f(b.value[a.name]["通信状态"]?b.value[a.name]["通信状态"]:""),1)])),_:2},1024),m(s,{span:14},{default:u((()=>[c("div",D,f(b.value[a.name]["数据更新时间戳"]?b.value[a.name]["数据更新时间戳"]:b.value[a.name]["数据更新时间"]?b.value[a.name]["数据更新时间"]:""),1)])),_:2},1024)])),_:2},1024)])])),_:2},1024),m(s,{span:12},{default:u((()=>[m(i,{data:a.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:u((()=>[m(t,{type:"index",width:"50",align:"center",label:"序号"}),m(t,{prop:"name",label:"数据名称",align:"center"}),m(t,{prop:"value",label:"数值",align:"center"}),m(t,{prop:"unit",label:"单位",align:"center",width:"70"})])),_:2},1032,["data"])])),_:2},1024)])),_:2},1024)])),_:2},1024)))),128))])),_:1})])}}}),[["__scopeId","data-v-4d6368be"]]);export{O as default};

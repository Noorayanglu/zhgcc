import{a}from"./user-1c5c1bdb.js";import{j as e}from"./index-f17869a5.js";import{g as s}from"./utils-e10d97cb.js";import{d as l,e as t,i,Y as o,ag as r,c as n,U as m,O as u,o as p,a as d,J as c,T as v,az as j,aA as f}from"./@vue-c5a747fc.js";import{_ as g}from"./index-ac98f9a0.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./feedback-5363c0cc.js";import"./element-plus-230cec57.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const b=a=>(j("data-v-456b5d8f"),a=a(),f(),a),_={class:"bms"},x={class:"img-data"},h=b((()=>d("div",{class:"img-box"},[d("img",{src:"/static/png/bms-899464a2.png",alt:""})],-1))),w={class:"imgdata-box"},y=b((()=>d("div",{class:"imgdata-name"},"通信状态",-1))),I=b((()=>d("div",{class:"imgdata-name"},"数据更新时间",-1))),z={class:"imgdata-value"},C={class:"tab-box"},A=g(l({__name:"Index",setup(l){const j=t([]),f=t({});t(!0),t(null),t(null),t(null),t(null),t([]);const g=t(""),b=t(""),A=t(null),F=t(2e3);i((()=>{E()})),o((()=>{k()}));const k=()=>{clearInterval(A.value),A.value=null},E=()=>{a().then((a=>{const{"数据更新时间":l,"通信状态":t,...i}=a;f.value=i,g.value=l,b.value=t,j.value=e(f.value),j.value.map((a=>(a.value=s(a.value),a))),k(),A.value=setInterval((()=>{E()}),F.value)})).catch((a=>{}))};return(a,e)=>{const s=r("el-col"),l=r("el-row"),t=r("el-table-column"),i=r("el-table");return p(),n("div",_,[m(l,null,{default:u((()=>[m(s,{xs:24,sm:12},{default:u((()=>[d("div",x,[h,d("div",w,[m(l,null,{default:u((()=>[m(s,{span:10},{default:u((()=>[y])),_:1}),m(s,{span:14},{default:u((()=>[I])),_:1}),m(s,{span:10},{default:u((()=>[d("div",{class:"imgdata-value",style:c("color:".concat("正常"==b.value?"#67C23A":"#F56C6C",";"))},v(b.value),5)])),_:1}),m(s,{span:14},{default:u((()=>[d("div",z,v(g.value),1)])),_:1})])),_:1})])])])),_:1}),m(s,{xs:24,sm:12},{default:u((()=>[d("div",C,[m(i,{data:j.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:u((()=>[m(t,{type:"index",width:"60",align:"center",label:"序号"}),m(t,{prop:"name",label:"数据名称",align:"center"}),m(t,{prop:"value",label:"数值",align:"center"}),m(t,{prop:"unit",label:"单位",align:"center",width:"80"})])),_:1},8,["data"])])])),_:1})])),_:1})])}}}),[["__scopeId","data-v-456b5d8f"]]);export{A as default};

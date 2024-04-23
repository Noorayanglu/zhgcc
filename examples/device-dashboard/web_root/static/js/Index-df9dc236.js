import{c as e,d as l,e as a,f as t}from"./user-c79d2ac7.js";import{j as o}from"./index-12783443.js";import{g as s}from"./utils-e10d97cb.js";import{f as d}from"./feedback-551b2bff.js";import{d as m,e as p,i as u,Y as r,Z as n,ag as i,c,U as f,O as v,o as b,a as h,J as g,T as _,F as y,a7 as j,M as x,u as V,S as w,az as k,aA as U}from"./@vue-c5a747fc.js";import{_ as C}from"./index-b41c9e41.js";import"./index-a06b32b6.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-a5ae5727.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const z=[{name:"停机",value:0},{name:"制冷",value:1},{name:"加热",value:2},{name:"自循环",value:3}],N=[{name:"手动",value:0},{name:"电芯温度",value:1},{name:"回水温度",value:2}],E=e=>(k("data-v-fc58c7f4"),e=e(),U(),e),S={class:"cooling"},F={class:"img-data"},I=E((()=>h("div",{class:"img-box"},[h("img",{src:"/static/png/cooling-2157892c.png",alt:""})],-1))),R={class:"imgdata-box"},A=E((()=>h("div",{class:"imgdata-name"},"通信状态",-1))),B={class:"btn-box"},D={class:"card-box"},J=E((()=>h("div",{class:"card-header"},[h("span",null,"控制")],-1))),M={class:"first-form"},O={class:"first-btn"},T={class:"first-form"},Y={class:"two-box"},Z={class:"first-btn"},q={class:"first-form"},G={class:"two-box"},H={class:"first-btn"},K={class:"tab-box"},L=C(m({__name:"Index",setup(m){const k=p([]),U=p({}),C=z,E=N;p(0);const L=p("");p(!0);const P=p(null);p(2e3),u((()=>{te()})),r((()=>{ae()}));const Q=p(),W=n({mode:null,temp:null,setEnabled:!1}),X=p(),$=n({ctlmod:null}),ee=p(),le=n({heattemp:null,heatgap:null,cooltemp:null,coolgap:null}),ae=()=>{clearInterval(P.value),P.value=null},te=()=>{e().then((e=>{const{"通信状态":l,...a}=e;U.value=a,L.value=l,k.value=o(U.value),k.value.map((e=>(e.value=s(e.value),e))),W.temp=a["温度设定"],W.mode="停机"===a["工作模式"]?0:"制冷"===a["工作模式"]?1:"加热"===a["工作模式"]?2:3,$.ctlmod="手动"===a["控制模式"]?0:"电芯温度"===a["控制模式"]?1:2,le.heattemp=a["加热温度"],le.heatgap=a["加热回差"],le.cooltemp=a["制冷温度"],le.coolgap=a["制冷回差"]})).catch((e=>{}))};return(e,o)=>{const s=i("el-col"),m=i("el-row"),p=i("el-option"),u=i("el-select"),r=i("el-form-item"),n=i("el-input"),U=i("el-button"),z=i("el-form"),N=i("el-divider"),P=i("el-card"),ae=i("el-table-column"),te=i("el-table");return b(),c("div",S,[f(m,null,{default:v((()=>[f(s,{span:12,class:"col-box"},{default:v((()=>[h("div",F,[I,h("div",R,[f(m,null,{default:v((()=>[f(s,{span:24},{default:v((()=>[A])),_:1}),f(s,{span:24},{default:v((()=>[h("div",{class:"imgdata-value",style:g("color:".concat("正常"==L.value?"#67C23A":"#F56C6C",";"))},_(L.value),5)])),_:1})])),_:1})])]),h("div",B,[h("div",D,[f(P,{class:"box-card"},{header:v((()=>[J])),default:v((()=>[h("div",null,[f(z,{ref_key:"formRef",ref:Q,model:W,size:"default"},{default:v((()=>[h("div",M,[h("div",null,[f(r,{label:"工作模式",prop:"mode"},{default:v((()=>[f(u,{modelValue:W.mode,"onUpdate:modelValue":o[0]||(o[0]=e=>W.mode=e),placeholder:"工作模式",style:{width:"208px"}},{default:v((()=>[(b(!0),c(y,null,j(V(C),((e,l)=>(b(),x(p,{key:l,label:e.name,value:e.value},null,8,["label","value"])))),128))])),_:1},8,["modelValue"])])),_:1}),f(r,{label:"温度设定",prop:"temp"},{default:v((()=>[f(n,{modelValue:W.temp,"onUpdate:modelValue":o[1]||(o[1]=e=>W.temp=e),type:"number",style:{width:"208px"}},{append:v((()=>[w("℃")])),_:1},8,["modelValue"])])),_:1})]),h("div",O,[f(U,{class:"btn",type:"primary",onClick:o[2]||(o[2]=e=>(e=>{if(!e)return;if(null===W.temp||""===W.temp)return void d.msgError("请输入温度");let l={mode:W.mode,temp:Number(W.temp)};t(l).then((e=>{d.msgSuccess("设置成功")})).catch((e=>{}))})(Q.value))},{default:v((()=>[w("设置")])),_:1})])])])),_:1},8,["model"]),f(N,{"border-style":"dashed",class:"oneline"}),f(z,{ref_key:"formRef1",ref:X,model:$,size:"default"},{default:v((()=>[h("div",T,[h("div",Y,[f(r,{label:"控制模式",prop:"ctlmod"},{default:v((()=>[f(u,{modelValue:$.ctlmod,"onUpdate:modelValue":o[3]||(o[3]=e=>$.ctlmod=e),placeholder:"控制模式",style:{width:"208px"}},{default:v((()=>[(b(!0),c(y,null,j(V(E),((e,l)=>(b(),x(p,{key:l,label:e.name,value:e.value},null,8,["label","value"])))),128))])),_:1},8,["modelValue"])])),_:1})]),h("div",Z,[f(U,{class:"btn",type:"primary",onClick:o[4]||(o[4]=e=>(e=>{if(!e)return;if(null===$.ctlmod)return void d.msgError("请选择控制模式");let l={ctlmod:$.ctlmod};a(l).then((e=>{d.msgSuccess("设置成功")})).catch((e=>{}))})(X.value))},{default:v((()=>[w("设置")])),_:1})])])])),_:1},8,["model"]),f(N,{"border-style":"dashed",class:"oneline"}),f(z,{ref_key:"formRef2",ref:ee,model:le,size:"default"},{default:v((()=>[h("div",q,[h("div",G,[f(r,{label:"加热温度",prop:"heattemp"},{default:v((()=>[f(n,{modelValue:le.heattemp,"onUpdate:modelValue":o[5]||(o[5]=e=>le.heattemp=e),type:"number",style:{width:"208px"}},{append:v((()=>[w("℃")])),_:1},8,["modelValue"])])),_:1}),f(r,{label:"加热回差",prop:"heatgap"},{default:v((()=>[f(n,{modelValue:le.heatgap,"onUpdate:modelValue":o[6]||(o[6]=e=>le.heatgap=e),type:"number",style:{width:"208px"}},{append:v((()=>[w("℃")])),_:1},8,["modelValue"])])),_:1}),f(r,{label:"制冷温度",prop:"cooltemp"},{default:v((()=>[f(n,{modelValue:le.cooltemp,"onUpdate:modelValue":o[7]||(o[7]=e=>le.cooltemp=e),type:"number",style:{width:"208px"}},{append:v((()=>[w("℃")])),_:1},8,["modelValue"])])),_:1}),f(r,{label:"制冷回差",prop:"coolgap"},{default:v((()=>[f(n,{modelValue:le.coolgap,"onUpdate:modelValue":o[8]||(o[8]=e=>le.coolgap=e),type:"number",style:{width:"208px"}},{append:v((()=>[w("℃")])),_:1},8,["modelValue"])])),_:1})]),h("div",H,[f(U,{class:"btn",type:"primary",onClick:o[9]||(o[9]=e=>(e=>{if(!e)return;let a={heattemp:Number(le.heattemp),heatgap:Number(le.heatgap),cooltemp:Number(le.cooltemp),coolgap:Number(le.coolgap)};l(a).then((e=>{d.msgSuccess("设置成功")})).catch((e=>{}))})(ee.value))},{default:v((()=>[w("设置")])),_:1})])])])),_:1},8,["model"])])])),_:1})])])])),_:1}),f(s,{span:12},{default:v((()=>[h("div",K,[f(te,{data:k.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:v((()=>[f(ae,{type:"index",width:"60",align:"center",label:"序号"}),f(ae,{prop:"name",label:"数据名称",align:"center"}),f(ae,{prop:"value",label:"数值",align:"center"}),f(ae,{prop:"unit",label:"单位",align:"center",width:"80"})])),_:1},8,["data"])])])),_:1})])),_:1})])}}}),[["__scopeId","data-v-fc58c7f4"]]);export{L as default};
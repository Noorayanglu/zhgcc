import{n as e,o as a}from"./user-643510a0.js";import{f as r}from"./feedback-551b2bff.js";import{d as s,i as t,e as l,Z as o,ag as m,c as p,U as i,O as u,o as n,a as d,S as c,T as f}from"./@vue-c5a747fc.js";import{_ as j}from"./index-05dec863.js";import"./index-4c7d2c62.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-a5ae5727.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const w={class:"setup"},v={class:"newpower-box"},b={class:"newpower-text"},h={class:"newpower-box"},_={class:"newpower-text"},y=j(s({__name:"Index",setup(s){t((()=>{k()}));const j=l({}),y=l(),x=o({param:null}),g=l(),V=o({param:null}),k=()=>{e().then((e=>{j.value=e})).catch((e=>{}))};return(e,s)=>{const t=m("el-input"),l=m("el-button"),o=m("el-form-item"),z=m("el-form");return n(),p("div",w,[i(z,{ref_key:"formRef",ref:y,model:x,size:"default"},{default:u((()=>[i(o,{label:"充电截至电芯电压",prop:"param"},{default:u((()=>[d("div",v,[i(t,{modelValue:x.param,"onUpdate:modelValue":s[0]||(s[0]=e=>x.param=e),type:"number",class:"newpower-input",style:{width:"208px"}},null,8,["modelValue"]),i(l,{class:"newpower-btn",type:"primary",onClick:s[1]||(s[1]=e=>(e=>{if(!e)return;if(null===x.param)return void r.msgError("请输入充电截至电芯电压");let s={cmd:0,param:1e3*Number(x.param)};a(s).then((e=>{r.msgSuccess("设置成功"),k()})).catch((e=>{}))})(y.value))},{default:u((()=>[c("设置")])),_:1})]),d("div",b,"当前值："+f(j.value["充电截至电芯电压"]),1)])),_:1})])),_:1},8,["model"]),i(z,{ref_key:"formRef1",ref:g,model:V,size:"default"},{default:u((()=>[i(o,{label:"放电截至电芯电压",prop:"param"},{default:u((()=>[d("div",h,[i(t,{modelValue:V.param,"onUpdate:modelValue":s[2]||(s[2]=e=>V.param=e),type:"number",class:"newpower-input",style:{width:"208px"}},null,8,["modelValue"]),i(l,{class:"newpower-btn",type:"primary",onClick:s[3]||(s[3]=e=>(e=>{if(!e)return;if(null===V.param)return void r.msgError("请输入放电截至电芯电压");let s={cmd:1,param:1e3*Number(V.param)};a(s).then((e=>{r.msgSuccess("设置成功"),k()})).catch((e=>{}))})(g.value))},{default:u((()=>[c("设置")])),_:1})]),d("div",_,"当前值："+f(j.value["放电截至电芯电压"]),1)])),_:1})])),_:1},8,["model"])])}}}),[["__scopeId","data-v-c477777d"]]);export{y as default};

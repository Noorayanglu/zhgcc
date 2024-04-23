import{n as e,o as a}from"./user-2715e817.js";import{j as t}from"./index-a958ca52.js";import{f as l}from"./feedback-5363c0cc.js";import{i as r}from"./echarts-53b3fd57.js";import{_ as s}from"./index-f2a4abfe.js";import{i as o,f as n,Y as i,e as u,c as d,a as m,o as p,d as c,Z as f,ag as v,U as h,O as b,S as g,F as w,a7 as y,u as _,az as x,aA as j}from"./@vue-c5a747fc.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-230cec57.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./zrender-299a952d.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const T={class:"echarts-box"},V=s({__name:"setPower",props:{chartseries:{type:Array,default:[]}},setup(e){const a=e;o((()=>{s()})),n((()=>a.chartseries),(()=>{s()}),{deep:!0}),i((()=>{l.dispose}));const t=u();let l;function s(){l=r(t.value);const e={tooltip:{trigger:"axis",axisPointer:{type:"line"},backgroundColor:"rgba(0,0,0,0.5)",borderColor:"rgba(0,0,0,0.5)",textStyle:{color:"#fff"}},legend:{textStyle:{}},grid:{top:"20%",left:"3%",right:"4%",bottom:"3%",containLabel:!0},xAxis:{type:"category",boundaryGap:!1,axisTick:{show:!1},axisLabel:{},axisLine:{},data:""},yAxis:{type:"value",axisLine:{show:!0,lineStyle:{}},axisPointer:{type:"none"}},series:[{name:"功率曲线",type:"line",showSymbol:!1,data:a.chartseries}]};l.setOption(e)}return(e,a)=>(p(),d("div",T,[m("div",{ref_key:"myEcharts",ref:t,style:{width:"100%",height:"100%"}},null,512)]))}},[["__scopeId","data-v-18398d5f"]]),S=e=>(x("data-v-621cf683"),e=e(),j(),e),k={class:"setup"},P={class:"tab-box"},C=S((()=>m("div",{class:"card-header"},[m("span",null,"控制")],-1))),U={class:"newpower-box"},E={class:"newpower-box"},I=S((()=>m("div",{class:"card-header"},[m("span",null,"功率曲线设置")],-1))),z={class:"card-con"},A=S((()=>m("div",{class:"power-name"},"时间",-1))),L=S((()=>m("div",{class:"power-name power-text-margin"},"功率值（KW）",-1))),N={class:"setvalue-value"},O=S((()=>m("template",{slot:"append"},[g("KW")],-1))),D=["onClick"],F={class:"setpowerbtn"},G={class:"power-box"},K={class:"power-box-box"},M=s(c({__name:"Index",setup(r){const s=u([]),n=u({}),i=u([]),c=u([]),x=u([]),j=u(15);u("");let T=f([{chargeP:"",startTime:"",endTime:""}]);const S=u(),M=f({param:null}),R=u(),W=f({param:null});o((async()=>{await ee(),c.value=await X(j.value),J(c.value,i.value)}));const Y=async()=>{let e=await q();J(c.value,Object.values(e))},Z=async()=>{let e=await q();e&&B(e)},q=()=>{let e=T.map(((e,a)=>[e.startTime,e.endTime]));if(T.some((e=>""===e.chargeP||""===e.startTime||""===e.endTime)))return void l.msgError("功率值与时间不能为空");if(re(e))return void l.msgError("时间段有重复");let a=te(T);return ae(a).reduce(((e,a,t)=>(e[t+1]=a.chargeP,e)),{})},B=e=>{a({cmd:2,param:e}).then((e=>{l.msgSuccess("设置成功"),ee()})).catch((e=>{}))},H=()=>{T.push({chargeP:"",startTime:"",endTime:""})},J=async(e,a)=>{let t=await Q(e,a);c.value=t.map((e=>e.name)),i.value=t.map((e=>e.value)),x.value=c.value.map(((e,a)=>[e,i.value[a]]))},Q=(e,a)=>{const t=e.map(((e,t)=>({name:e,value:a[t]})));let l=[];return l.push(t[0]),t.reduce(((e,a)=>{let t=null;return e.value!==a.value&&(t={name:a.name,value:e.value}),t?l.push(t,a):l.push(a),a})),l},X=e=>{let a=60*e,t=86400/a,l=0,r=[];for(let s=0;s<t;s++){let e=parseInt(l/3600),t=parseInt(l%3600/60);r.push($(e)+":"+$(t)),l+=a}return r},$=e=>e<10?"0"+e:e,ee=()=>{e().then((e=>{let a=e;n.value={"充电截止电芯电压":a["充电截止电芯电压"],"放电截止电芯电压":a["放电截止电芯电压"],"数据记录状态":a["数据记录状态"]},s.value=t(n.value);const{"充电截止电芯电压":l,"放电截止电芯电压":r,"数据记录状态":o,...u}=a;i.value=Object.values(u)})).catch((e=>{}))},ae=e=>{let a=[];for(let t=0;t<1440;t+=15){const l="".concat(String(Math.floor(t/60)).padStart(2,"0"),":").concat(String(t%60).padStart(2,"0"));let r="0";for(const a of e){const e=le(a.startTime),l=le(a.endTime);if(t>=e&&t<l){r=a.chargeP;break}}a.push({time:l,chargeP:r})}return a},te=e=>(e.sort(((e,a)=>{const t=le(e.startTime),l=le(e.endTime),r=le(a.startTime),s=le(a.endTime);return t!==r?t-r:l-s})),e),le=e=>{const[a,t]=e.split(":").map(Number);return 60*a+t},re=e=>{const a=new Set;for(const[t,l]of e)for(let e=t;e<l;e=se(e,1)){if(a.has(e))return!0;a.add(e)}return!1},se=(e,a)=>{const[t,l]=e.split(":").map(Number);let r=t+Math.floor((l+a)/60),s=(l+a)%60;return 60===s&&(s=0,r+=1),"".concat(String(r).padStart(2,"0"),":").concat(String(s).padStart(2,"0"))};return(e,t)=>{const r=v("el-table-column"),o=v("el-table"),n=v("el-input"),i=v("el-button"),u=v("el-form-item"),c=v("el-form"),f=v("el-card"),j=v("el-col"),q=v("el-row"),B=v("el-time-select"),J=v("Delete"),Q=v("el-icon"),X=v("Plus");return p(),d("div",k,[m("div",null,[h(q,null,{default:b((()=>[h(j,{span:12,class:"col-box"},{default:b((()=>[m("div",P,[h(o,{data:s.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:b((()=>[h(r,{type:"index",width:"60",align:"center",label:"序号"}),h(r,{prop:"name",label:"数据名称",align:"center"}),h(r,{prop:"value",label:"数值",align:"center"}),h(r,{prop:"unit",label:"单位",align:"center",width:"60"})])),_:1},8,["data"])]),h(f,{class:"box-card"},{header:b((()=>[C])),default:b((()=>[m("div",null,[h(c,{ref_key:"formRef",ref:S,model:M},{default:b((()=>[h(u,{label:"充电截至电芯电压",prop:"param"},{default:b((()=>[m("div",U,[h(n,{modelValue:M.param,"onUpdate:modelValue":t[0]||(t[0]=e=>M.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),h(i,{class:"newpower-btn",type:"primary",onClick:t[1]||(t[1]=e=>(e=>{if(!e)return;if(null===M.param)return void l.msgError("请输入充电截至电芯电压");let t={cmd:0,param:1e3*Number(M.param)};a(t).then((e=>{l.msgSuccess("设置成功"),ee()})).catch((e=>{}))})(S.value))},{default:b((()=>[g("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),h(c,{ref_key:"formRef1",ref:R,model:W},{default:b((()=>[h(u,{label:"放电截至电芯电压",prop:"param"},{default:b((()=>[m("div",E,[h(n,{modelValue:W.param,"onUpdate:modelValue":t[2]||(t[2]=e=>W.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),h(i,{class:"newpower-btn",type:"primary",onClick:t[3]||(t[3]=e=>(e=>{if(!e)return;if(null===W.param)return void l.msgError("请输入放电截至电芯电压");let t={cmd:1,param:1e3*Number(W.param)};a(t).then((e=>{l.msgSuccess("设置成功"),ee()})).catch((e=>{}))})(R.value))},{default:b((()=>[g("设置")])),_:1})])])),_:1})])),_:1},8,["model"])])])),_:1})])),_:1}),h(j,{span:12},{default:b((()=>[h(f,{class:"box-card"},{header:b((()=>[I])),default:b((()=>[m("div",z,[h(q,null,{default:b((()=>[h(j,{span:14},{default:b((()=>[A])),_:1}),h(j,{span:8},{default:b((()=>[L])),_:1})])),_:1}),(p(!0),d(w,null,y(_(T),((e,a)=>(p(),d("div",{key:a},[m("div",N,[h(q,null,{default:b((()=>[h(j,{span:14,class:"col-box-time"},{default:b((()=>[h(B,{modelValue:e.startTime,"onUpdate:modelValue":a=>e.startTime=a,"max-time":e.endTime,placeholder:"起始时间",start:"00:00",step:"00:15",end:"24:45"},null,8,["modelValue","onUpdate:modelValue","max-time"]),g("至 "),h(B,{modelValue:e.endTime,"onUpdate:modelValue":a=>e.endTime=a,"min-time":e.startTime,placeholder:"结束时间",start:"00:00",step:"00:15",end:"24:45"},null,8,["modelValue","onUpdate:modelValue","min-time"])])),_:2},1024),h(j,{span:8},{default:b((()=>[h(n,{modelValue:e.chargeP,"onUpdate:modelValue":a=>e.chargeP=a,placeholder:"功率值",autocomplete:"off"},{default:b((()=>[O])),_:2},1032,["modelValue","onUpdate:modelValue"])])),_:2},1024),h(j,{span:2},{default:b((()=>[m("div",{class:"del-btn",onClick:e=>(e=>{T.splice(e,1)})(a)},[h(Q,null,{default:b((()=>[h(J)])),_:1})],8,D)])),_:2},1024)])),_:2},1024)])])))),128)),m("div",null,[m("div",{class:"add-btn",onClick:H},[h(Q,null,{default:b((()=>[h(X)])),_:1}),g(" 新增 ")])]),m("div",F,[h(i,{class:"newpower-btn",type:"primary",onClick:Z},{default:b((()=>[g("设置")])),_:1}),h(i,{class:"newpower-btn",type:"primary",onClick:Y},{default:b((()=>[g("功率图预览")])),_:1})])])])),_:1})])),_:1})])),_:1})]),m("div",G,[m("div",K,[h(V,{chartseries:x.value},null,8,["chartseries"])])])])}}}),[["__scopeId","data-v-621cf683"]]);export{M as default};

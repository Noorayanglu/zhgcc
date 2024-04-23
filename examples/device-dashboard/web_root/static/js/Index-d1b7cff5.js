import{m as e,n as a}from"./user-3b1a53c5.js";import{j as l}from"./index-c5194384.js";import{f as r}from"./feedback-5363c0cc.js";import{i as t}from"./echarts-84a2ba7b.js";import{_ as o}from"./index-d7b70977.js";import{i as s,f as m,z as n,e as u,o as d,c as p,a as i,d as c,Z as f,ag as w,U as b,O as h,S as v,F as g,a7 as _,u as x,az as y,aA as V}from"./@vue-c5a747fc.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-230cec57.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./zrender-1465eda9.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const E={class:"echarts-box"},k=o({__name:"setPower",props:{chartseries:{type:Array,default:[]}},setup(e){const a=e;s((()=>{o()})),m((()=>a.chartseries),(()=>{r&&r.dispose(),o()}),{deep:!0}),n((()=>{r&&r.dispose()}));const l=u();let r;function o(){r=t(l.value);const e={title:{text:"功率曲线（正值：充电；负值：放电）",textStyle:{fontSize:14,fontWeight:400},top:"10px",left:"center"},tooltip:{trigger:"axis",title:"功率曲线",axisPointer:{type:"line"},backgroundColor:"rgba(0,0,0,0.5)",borderColor:"rgba(0,0,0,0.5)",textStyle:{color:"#fff"}},legend:{textStyle:{}},grid:{top:"13%",left:"3%",right:"3%",bottom:"3%",containLabel:!0},xAxis:{type:"category",boundaryGap:!1,axisTick:{show:!1},axisLabel:{},axisLine:{},data:""},yAxis:{type:"value",axisLine:{show:!0,lineStyle:{}},axisPointer:{type:"none"}},series:[{type:"line",showSymbol:!1,data:a.chartseries}]};r.setOption(e)}return(e,a)=>(d(),p("div",E,[i("div",{ref_key:"myEcharts",ref:l,style:{width:"100%",height:"100%"}},null,512)]))}},[["__scopeId","data-v-d86a0184"]]),C=e=>(y("data-v-eb0da820"),e=e(),V(),e),j={class:"setup"},S={class:"tab-box"},U=C((()=>i("div",{class:"card-header"},[i("span",null,"控制")],-1))),T={class:"newpower-box"},z={class:"newpower-box"},P={class:"newpower-box"},R={class:"newpower-box"},N={class:"newpower-box"},I={class:"newpower-box"},A={class:"newpower-box"},L={class:"newpower-box"},O={class:"newpower-box"},W={class:"newpower-box"},G={class:"newpower-box"},M=C((()=>i("div",{class:"card-header"},[i("span",null,"功率曲线设置")],-1))),D={class:"card-con"},F=C((()=>i("div",{class:"power-name time-text-margin"},"时间",-1))),K=C((()=>i("div",{class:"power-name power-text-margin"},"功率值（KW）",-1))),Z={class:"setvalue-value"},q=C((()=>i("template",{slot:"append"},[v("kW")],-1))),B={class:"setpowerbtn"},H={class:"power-box"},J={class:"power-box-box"},Q=o(c({__name:"Index",setup(t){const o=u([]),m=u({}),n=u([]),c=u([]),y=u([]),V=u(15);u("");let E=f([{chargeP:"",startTime:"",endTime:""}]);const C=u(),Q=f({param:""}),X=u(),Y=f({param:null}),$=u(),ee=f({param:null}),ae=u(),le=f({param:null}),re=u(),te=f({param:null}),oe=u(),se=f({param:null}),me=u(),ne=f({param:null}),ue=u(),de=f({param:null}),pe=u(),ie=f({param:null}),ce=u(),fe=f({param:null}),we=u(),be=f({param:null});s((async()=>{c.value=await Ve(V.value),await ke()}));const he=()=>{xe(c.value,n.value)},ve=()=>{ge().then((e=>{if(e){_e((e=>{let a=e,l=n.value.reduce(((e,a,l)=>(e[l+1]=a,e)),{});for(let r in a)"false"===a[r]&&(a[r]=l[r]),a[r]=Number(a[r]);return a})(e))}})).catch((e=>{}))},ge=()=>new Promise(((e,a)=>{let l=E.map(((e,a)=>[e.startTime,e.endTime]));if(E.some((e=>""===e.chargeP||""===e.startTime||""===e.endTime)))return r.msgError("功率值与时间不能为空"),void a(new Error("功率值与时间不能为空"));if(Ue(l))return r.msgError("时间段有重复"),void a(new Error("时间段有重复"));let t=je(E);t.forEach((e=>{"00:00"===e.endTime&&(e.endTime="24:00")})),e(Ce(t).reduce(((e,a,l)=>(e[l+1]=a.chargeP,e)),{}))})),_e=e=>{let l={cmd:2,...e};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))},xe=async(e,a)=>{let l=await ye(e,a),r=l.map((e=>e.name)),t=l.map((e=>e.value));y.value=r.map(((e,a)=>[e,t[a]]))},ye=(e,a)=>{const l=e.map(((e,l)=>({name:e,value:a[l]})));let r=[];return r.push(l[0]),l.reduce(((e,a)=>{let l=null;return e.value!==a.value&&(l={name:a.name,value:e.value}),l?r.push(l,a):r.push(a),a})),r},Ve=e=>{let a=60*e,l=86400/a,r=0,t=[];for(let o=0;o<l;o++){let e=parseInt(r/3600),l=parseInt(r%3600/60);t.push(Ee(e)+":"+Ee(l)),r+=a}return t},Ee=e=>e<10?"0"+e:e,ke=()=>{e().then((e=>{let a=e;m.value={"序列号":a["序列号"],"充电截止电芯电压":a["充电截止电芯电压"],"放电截止电芯电压":a["放电截止电芯电压"],"充电关口限制":a["充电关口限制"],"放电关口限制":a["放电关口限制"],"充电变压器限制":a["充电变压器限制"],"放电变压器限制":a["放电变压器限制"],"Cloud 用户名":a["Cloud用户名"],"Cloud 密码":a["Cloud密码"],"Cloud Url":a["Cloud Url"]},o.value=l(m.value);const{"序列号":r,"充电截止电芯电压":t,"放电截止电芯电压":s,"充电关口限制":u,"放电关口限制":d,"充电变压器限制":p,"放电变压器限制":i,"Cloud用户名":f,"Cloud密码":w,"Cloud Url":b,...h}=a;n.value=Object.values(h),xe(c.value,n.value)})).catch((e=>{}))},Ce=e=>{let a=[];for(let l=0;l<1440;l+=15){const r="".concat(String(Math.floor(l/60)).padStart(2,"0"),":").concat(String(l%60).padStart(2,"0"));let t="false";for(const a of e){const e=Se(a.startTime),r=Se(a.endTime);if(l>=e&&l<r){t=a.chargeP;break}}a.push({time:r,chargeP:t})}return a},je=e=>(e.sort(((e,a)=>{const l=Se(e.startTime),r=Se(e.endTime),t=Se(a.startTime),o=Se(a.endTime);return l!==t?l-t:r-o})),e),Se=e=>{const[a,l]=e.split(":").map(Number);return 60*a+l},Ue=e=>{const a=new Set;for(const[l,r]of e)for(let e=l;e<r;e=Te(e,1)){if(a.has(e))return!0;a.add(e)}return!1},Te=(e,a)=>{const[l,r]=e.split(":").map(Number);let t=l+Math.floor((r+a)/60),o=(r+a)%60;return 60===o&&(o=0,t+=1),"".concat(String(t).padStart(2,"0"),":").concat(String(o).padStart(2,"0"))};return(e,l)=>{const t=w("el-table-column"),s=w("el-table"),m=w("el-col"),n=w("el-input"),u=w("el-button"),c=w("el-form-item"),f=w("el-form"),V=w("el-card"),ge=w("el-row"),_e=w("el-time-select");return d(),p("div",j,[i("div",null,[b(ge,null,{default:h((()=>[b(m,{xs:24,sm:12,class:"col-box"},{default:h((()=>[i("div",S,[b(s,{data:o.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:h((()=>[b(t,{type:"index",width:"60",align:"center",label:"序号"}),b(t,{prop:"name",label:"数据名称",align:"center"}),b(t,{prop:"value",label:"数值",align:"center"}),b(t,{prop:"unit",label:"单位",align:"center",width:"60"})])),_:1},8,["data"])])])),_:1}),b(m,{xs:24,sm:12},{default:h((()=>[b(V,{class:"box-card tabline"},{header:h((()=>[U])),default:h((()=>[i("div",null,[b(f,{ref_key:"formRef0",ref:C,model:Q,"label-width":"130"},{default:h((()=>[b(c,{label:"序列号",prop:"param"},{default:h((()=>[i("div",T,[b(n,{modelValue:Q.param,"onUpdate:modelValue":l[0]||(l[0]=e=>Q.param=e),class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[1]||(l[1]=e=>(e=>{if(!e)return;if(""===Q.param)return void r.msgError("请输入序列号");let l={cmd:100,param:Q.param};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(C.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef",ref:X,model:Y,"label-width":"130"},{default:h((()=>[b(c,{label:"充电截至电芯电压",prop:"param"},{default:h((()=>[i("div",z,[b(n,{modelValue:Y.param,"onUpdate:modelValue":l[2]||(l[2]=e=>Y.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[3]||(l[3]=e=>(e=>{if(!e)return;if(null===Y.param)return void r.msgError("请输入充电截至电芯电压");let l={cmd:0,param:1e3*Number(Y.param)};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(X.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef1",ref:$,model:ee,"label-width":"130"},{default:h((()=>[b(c,{label:"放电截至电芯电压",prop:"param"},{default:h((()=>[i("div",P,[b(n,{modelValue:ee.param,"onUpdate:modelValue":l[4]||(l[4]=e=>ee.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[5]||(l[5]=e=>(e=>{if(!e)return;if(null===ee.param)return void r.msgError("请输入放电截至电芯电压");let l={cmd:1,param:1e3*Number(ee.param)};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})($.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef2",ref:ae,model:le,"label-width":"130"},{default:h((()=>[b(c,{label:"充电关口限制",prop:"param"},{default:h((()=>[i("div",R,[b(n,{modelValue:le.param,"onUpdate:modelValue":l[6]||(l[6]=e=>le.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[7]||(l[7]=e=>(e=>{if(!e)return;if(null===le.param)return void r.msgError("请输入充电关口限制");let l={cmd:3,param:Number(le.param)};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(ae.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef3",ref:re,model:te,"label-width":"130"},{default:h((()=>[b(c,{label:"放电关口限制",prop:"param"},{default:h((()=>[i("div",N,[b(n,{modelValue:te.param,"onUpdate:modelValue":l[8]||(l[8]=e=>te.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[9]||(l[9]=e=>(e=>{if(!e)return;if(null===te.param)return void r.msgError("请输入放电关口限制");let l={cmd:5,param:Number(te.param)};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(re.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef4",ref:oe,model:se,"label-width":"130"},{default:h((()=>[b(c,{label:"充电变压器限制",prop:"param"},{default:h((()=>[i("div",I,[b(n,{modelValue:se.param,"onUpdate:modelValue":l[10]||(l[10]=e=>se.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[11]||(l[11]=e=>(e=>{if(!e)return;if(null===se.param)return void r.msgError("请输入充电变压器限制");let l={cmd:4,param:Number(se.param)};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(oe.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef5",ref:me,model:ne,"label-width":"130"},{default:h((()=>[b(c,{label:"放电变压器限制",prop:"param"},{default:h((()=>[i("div",A,[b(n,{modelValue:ne.param,"onUpdate:modelValue":l[12]||(l[12]=e=>ne.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[13]||(l[13]=e=>(e=>{if(!e)return;if(null===ne.param)return void r.msgError("请输入放电变压器限制");let l={cmd:6,param:Number(ne.param)};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(me.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef6",ref:ue,model:de,"label-width":"130"},{default:h((()=>[b(c,{label:"Cloud 用户名",prop:"param"},{default:h((()=>[i("div",L,[b(n,{modelValue:de.param,"onUpdate:modelValue":l[14]||(l[14]=e=>de.param=e),class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[15]||(l[15]=e=>(e=>{if(!e)return;if(null===de.param)return void r.msgError("请输入Cloud 用户名");let l={cmd:101,param:de.param};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(ue.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef7",ref:pe,model:ie,"label-width":"130"},{default:h((()=>[b(c,{label:"Cloud 密码",prop:"param"},{default:h((()=>[i("div",O,[b(n,{modelValue:ie.param,"onUpdate:modelValue":l[16]||(l[16]=e=>ie.param=e),class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[17]||(l[17]=e=>(e=>{if(!e)return;if(null===ie.param)return void r.msgError("请输入Cloud 密码");let l={cmd:102,param:ie.param};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(pe.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef8",ref:ce,model:fe,"label-width":"130"},{default:h((()=>[b(c,{label:"Cloud Url",prop:"param"},{default:h((()=>[i("div",W,[b(n,{modelValue:fe.param,"onUpdate:modelValue":l[18]||(l[18]=e=>fe.param=e),class:"newpower-input"},null,8,["modelValue"]),b(u,{class:"newpower-btn",onClick:l[19]||(l[19]=e=>(e=>{if(!e)return;if(null===fe.param)return void r.msgError("请输入Cloud Url");let l={cmd:103,param:fe.param};a(l).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(ce.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),b(f,{ref_key:"formRef9",ref:we,model:be,"label-width":"130"},{default:h((()=>[b(c,{label:"",prop:"param"},{default:h((()=>[i("div",G,[b(u,{class:"newpower-btn",onClick:l[20]||(l[20]=e=>(e=>{if(!e)return;a({cmd:104,param:0}).then((e=>{e?r.msgSuccess("操作成功"):r.msgError("操作失败"),ke()})).catch((e=>{r.msgError("操作失败")}))})(we.value))},{default:h((()=>[v("Cloud 注册")])),_:1})])])),_:1})])),_:1},8,["model"])])])),_:1})])),_:1}),b(m,{span:24,class:"power-card"},{default:h((()=>[b(V,{class:"box-card"},{header:h((()=>[M])),default:h((()=>[i("div",D,[b(ge,null,{default:h((()=>[b(m,{span:16},{default:h((()=>[F])),_:1}),b(m,{span:8},{default:h((()=>[K])),_:1})])),_:1}),(d(!0),p(g,null,_(x(E),((e,a)=>(d(),p("div",{key:a},[i("div",Z,[b(ge,null,{default:h((()=>[b(m,{span:16,class:"col-box-time"},{default:h((()=>[b(_e,{modelValue:e.startTime,"onUpdate:modelValue":a=>e.startTime=a,"max-time":e.endTime,placeholder:"起始时间",start:"00:00",step:"00:15",end:"24:00",size:"default",editable:!1},null,8,["modelValue","onUpdate:modelValue","max-time"]),v("至 "),b(_e,{modelValue:e.endTime,"onUpdate:modelValue":a=>e.endTime=a,"min-time":e.startTime,placeholder:"结束时间",start:"00:00",step:"00:15",end:"24:00",size:"default",editable:!1},null,8,["modelValue","onUpdate:modelValue","min-time"])])),_:2},1024),b(m,{span:8},{default:h((()=>[b(n,{modelValue:e.chargeP,"onUpdate:modelValue":a=>e.chargeP=a,placeholder:"功率值",size:"default",autocomplete:"off"},{default:h((()=>[q])),_:2},1032,["modelValue","onUpdate:modelValue"])])),_:2},1024)])),_:2},1024)])])))),128)),i("div",B,[b(u,{class:"newpower-btn",size:"default",onClick:ve},{default:h((()=>[v("设置")])),_:1}),b(u,{class:"newpower-btn",size:"default",onClick:he},{default:h((()=>[v("查看当前功率曲线")])),_:1})])])])),_:1})])),_:1}),b(m,{span:24},{default:h((()=>[i("div",H,[i("div",J,[b(k,{chartseries:y.value},null,8,["chartseries"])])])])),_:1})])),_:1})])])}}}),[["__scopeId","data-v-eb0da820"]]);export{Q as default};

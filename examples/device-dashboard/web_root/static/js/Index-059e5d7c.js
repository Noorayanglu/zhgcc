import{l as e,m as a}from"./user-17eb7a52.js";import{j as r}from"./index-796d66c9.js";import{f as l}from"./feedback-5363c0cc.js";import{i as t}from"./echarts-84a2ba7b.js";import{_ as s}from"./index-5aae5289.js";import{i as o,f as m,z as n,e as p,o as u,c as d,a as i,d as c,Z as f,ag as b,U as w,O as h,S as v,F as g,a7 as y,u as _,az as x,aA as V}from"./@vue-c5a747fc.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-230cec57.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./zrender-1465eda9.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const E={class:"echarts-box"},j=s({__name:"setPower",props:{chartseries:{type:Array,default:[]}},setup(e){const a=e;o((()=>{s()})),m((()=>a.chartseries),(()=>{l&&l.dispose(),s()}),{deep:!0}),n((()=>{l&&l.dispose()}));const r=p();let l;function s(){l=t(r.value);const e={title:{text:"功率曲线（正值：充电；负值：放电）",textStyle:{fontSize:14,fontWeight:400},top:"10px",left:"center"},tooltip:{trigger:"axis",title:"功率曲线",axisPointer:{type:"line"},backgroundColor:"rgba(0,0,0,0.5)",borderColor:"rgba(0,0,0,0.5)",textStyle:{color:"#fff"}},legend:{textStyle:{}},grid:{top:"13%",left:"3%",right:"3%",bottom:"3%",containLabel:!0},xAxis:{type:"category",boundaryGap:!1,axisTick:{show:!1},axisLabel:{},axisLine:{},data:""},yAxis:{type:"value",axisLine:{show:!0,lineStyle:{}},axisPointer:{type:"none"}},series:[{type:"line",showSymbol:!1,data:a.chartseries}]};l.setOption(e)}return(e,a)=>(u(),d("div",E,[i("div",{ref_key:"myEcharts",ref:r,style:{width:"100%",height:"100%"}},null,512)]))}},[["__scopeId","data-v-d86a0184"]]),S=e=>(x("data-v-54eb8e22"),e=e(),V(),e),k={class:"setup"},T={class:"tab-box"},U=S((()=>i("div",{class:"card-header"},[i("span",null,"控制")],-1))),z={class:"newpower-box"},C={class:"newpower-box"},P={class:"newpower-box"},N={class:"newpower-box"},R={class:"newpower-box"},I={class:"newpower-box"},A={class:"newpower-box"},L=S((()=>i("div",{class:"card-header"},[i("span",null,"功率曲线设置")],-1))),O={class:"card-con"},W=S((()=>i("div",{class:"power-name time-text-margin"},"时间",-1))),G=S((()=>i("div",{class:"power-name power-text-margin"},"功率值（KW）",-1))),M={class:"setvalue-value"},F=S((()=>i("template",{slot:"append"},[v("kW")],-1))),H={class:"setpowerbtn"},K={class:"power-box"},Z={class:"power-box-box"},q=s(c({__name:"Index",setup(t){const s=p([]),m=p({}),n=p([]),c=p([]),x=p([]),V=p(15);p("");let E=f([{chargeP:"",startTime:"",endTime:""}]);const S=p(),q=f({param:""}),B=p(),D=f({param:null}),J=p(),Q=f({param:null}),X=p(),Y=f({param:null}),$=p(),ee=f({param:null}),ae=p(),re=f({param:null}),le=p(),te=f({param:null});o((async()=>{c.value=await de(V.value),await ce()}));const se=()=>{pe(c.value,n.value)},oe=()=>{me().then((e=>{if(e){ne((e=>{let a=e,r=n.value.reduce(((e,a,r)=>(e[r+1]=a,e)),{});for(let l in a)"false"===a[l]&&(a[l]=r[l]),a[l]=Number(a[l]);return a})(e))}})).catch((e=>{}))},me=()=>new Promise(((e,a)=>{let r=E.map(((e,a)=>[e.startTime,e.endTime]));if(E.some((e=>""===e.chargeP||""===e.startTime||""===e.endTime)))return l.msgError("功率值与时间不能为空"),void a(new Error("功率值与时间不能为空"));if(he(r))return l.msgError("时间段有重复"),void a(new Error("时间段有重复"));let t=be(E);t.forEach((e=>{"00:00"===e.endTime&&(e.endTime="24:00")})),e(fe(t).reduce(((e,a,r)=>(e[r+1]=a.chargeP,e)),{}))})),ne=e=>{let r={cmd:2,...e};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))},pe=async(e,a)=>{let r=await ue(e,a),l=r.map((e=>e.name)),t=r.map((e=>e.value));x.value=l.map(((e,a)=>[e,t[a]]))},ue=(e,a)=>{const r=e.map(((e,r)=>({name:e,value:a[r]})));let l=[];return l.push(r[0]),r.reduce(((e,a)=>{let r=null;return e.value!==a.value&&(r={name:a.name,value:e.value}),r?l.push(r,a):l.push(a),a})),l},de=e=>{let a=60*e,r=86400/a,l=0,t=[];for(let s=0;s<r;s++){let e=parseInt(l/3600),r=parseInt(l%3600/60);t.push(ie(e)+":"+ie(r)),l+=a}return t},ie=e=>e<10?"0"+e:e,ce=()=>{e().then((e=>{let a=e;m.value={"序列号":a["序列号"],"充电截止电芯电压":a["充电截止电芯电压"],"放电截止电芯电压":a["放电截止电芯电压"],"充电关口限制":a["充电关口限制"],"放电关口限制":a["放电关口限制"],"充电变压器限制":a["充电变压器限制"],"放电变压器限制":a["放电变压器限制"]},s.value=r(m.value);const{"序列号":l,"充电截止电芯电压":t,"放电截止电芯电压":o,"充电关口限制":p,"放电关口限制":u,"充电变压器限制":d,"放电变压器限制":i,...f}=a;n.value=Object.values(f),pe(c.value,n.value)})).catch((e=>{}))},fe=e=>{let a=[];for(let r=0;r<1440;r+=15){const l="".concat(String(Math.floor(r/60)).padStart(2,"0"),":").concat(String(r%60).padStart(2,"0"));let t="false";for(const a of e){const e=we(a.startTime),l=we(a.endTime);if(r>=e&&r<l){t=a.chargeP;break}}a.push({time:l,chargeP:t})}return a},be=e=>(e.sort(((e,a)=>{const r=we(e.startTime),l=we(e.endTime),t=we(a.startTime),s=we(a.endTime);return r!==t?r-t:l-s})),e),we=e=>{const[a,r]=e.split(":").map(Number);return 60*a+r},he=e=>{const a=new Set;for(const[r,l]of e)for(let e=r;e<l;e=ve(e,1)){if(a.has(e))return!0;a.add(e)}return!1},ve=(e,a)=>{const[r,l]=e.split(":").map(Number);let t=r+Math.floor((l+a)/60),s=(l+a)%60;return 60===s&&(s=0,t+=1),"".concat(String(t).padStart(2,"0"),":").concat(String(s).padStart(2,"0"))};return(e,r)=>{const t=b("el-table-column"),o=b("el-table"),m=b("el-col"),n=b("el-input"),p=b("el-button"),c=b("el-form-item"),f=b("el-form"),V=b("el-card"),me=b("el-row"),ne=b("el-time-select");return u(),d("div",k,[i("div",null,[w(me,null,{default:h((()=>[w(m,{xs:24,sm:12,class:"col-box"},{default:h((()=>[i("div",T,[w(o,{data:s.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:h((()=>[w(t,{type:"index",width:"60",align:"center",label:"序号"}),w(t,{prop:"name",label:"数据名称",align:"center"}),w(t,{prop:"value",label:"数值",align:"center"}),w(t,{prop:"unit",label:"单位",align:"center",width:"60"})])),_:1},8,["data"])])])),_:1}),w(m,{xs:24,sm:12},{default:h((()=>[w(V,{class:"box-card tabline"},{header:h((()=>[U])),default:h((()=>[i("div",null,[w(f,{ref_key:"formRef0",ref:S,model:q,"label-width":"130"},{default:h((()=>[w(c,{label:"序列号",prop:"param"},{default:h((()=>[i("div",z,[w(n,{modelValue:q.param,"onUpdate:modelValue":r[0]||(r[0]=e=>q.param=e),class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[1]||(r[1]=e=>(e=>{if(!e)return;if(""===q.param)return void l.msgError("请输入序列号");let r={cmd:100,param:q.param};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})(S.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),w(f,{ref_key:"formRef",ref:B,model:D,"label-width":"130"},{default:h((()=>[w(c,{label:"充电截至电芯电压",prop:"param"},{default:h((()=>[i("div",C,[w(n,{modelValue:D.param,"onUpdate:modelValue":r[2]||(r[2]=e=>D.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[3]||(r[3]=e=>(e=>{if(!e)return;if(null===D.param)return void l.msgError("请输入充电截至电芯电压");let r={cmd:0,param:1e3*Number(D.param)};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})(B.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),w(f,{ref_key:"formRef1",ref:J,model:Q,"label-width":"130"},{default:h((()=>[w(c,{label:"放电截至电芯电压",prop:"param"},{default:h((()=>[i("div",P,[w(n,{modelValue:Q.param,"onUpdate:modelValue":r[4]||(r[4]=e=>Q.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[5]||(r[5]=e=>(e=>{if(!e)return;if(null===Q.param)return void l.msgError("请输入放电截至电芯电压");let r={cmd:1,param:1e3*Number(Q.param)};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})(J.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),w(f,{ref_key:"formRef2",ref:X,model:Y,"label-width":"130"},{default:h((()=>[w(c,{label:"充电关口限制",prop:"param"},{default:h((()=>[i("div",N,[w(n,{modelValue:Y.param,"onUpdate:modelValue":r[6]||(r[6]=e=>Y.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[7]||(r[7]=e=>(e=>{if(!e)return;if(null===Y.param)return void l.msgError("请输入充电关口限制");let r={cmd:3,param:Number(Y.param)};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})(X.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),w(f,{ref_key:"formRef3",ref:$,model:ee,"label-width":"130"},{default:h((()=>[w(c,{label:"放电关口限制",prop:"param"},{default:h((()=>[i("div",R,[w(n,{modelValue:ee.param,"onUpdate:modelValue":r[8]||(r[8]=e=>ee.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[9]||(r[9]=e=>(e=>{if(!e)return;if(null===ee.param)return void l.msgError("请输入放电关口限制");let r={cmd:5,param:Number(ee.param)};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})($.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),w(f,{ref_key:"formRef4",ref:ae,model:re,"label-width":"130"},{default:h((()=>[w(c,{label:"充电变压器限制",prop:"param"},{default:h((()=>[i("div",I,[w(n,{modelValue:re.param,"onUpdate:modelValue":r[10]||(r[10]=e=>re.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[11]||(r[11]=e=>(e=>{if(!e)return;if(null===re.param)return void l.msgError("请输入充电变压器限制");let r={cmd:4,param:Number(re.param)};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})(ae.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"]),w(f,{ref_key:"formRef5",ref:le,model:te,"label-width":"130"},{default:h((()=>[w(c,{label:"放电变压器限制",prop:"param"},{default:h((()=>[i("div",A,[w(n,{modelValue:te.param,"onUpdate:modelValue":r[12]||(r[12]=e=>te.param=e),type:"number",class:"newpower-input"},null,8,["modelValue"]),w(p,{class:"newpower-btn",type:"primary",onClick:r[13]||(r[13]=e=>(e=>{if(!e)return;if(null===te.param)return void l.msgError("请输入放电变压器限制");let r={cmd:6,param:Number(te.param)};a(r).then((e=>{e?l.msgSuccess("操作成功"):l.msgError("操作失败"),ce()})).catch((e=>{l.msgError("操作失败")}))})(le.value))},{default:h((()=>[v("设置")])),_:1})])])),_:1})])),_:1},8,["model"])])])),_:1})])),_:1}),w(m,{span:24,class:"power-card"},{default:h((()=>[w(V,{class:"box-card"},{header:h((()=>[L])),default:h((()=>[i("div",O,[w(me,null,{default:h((()=>[w(m,{span:16},{default:h((()=>[W])),_:1}),w(m,{span:8},{default:h((()=>[G])),_:1})])),_:1}),(u(!0),d(g,null,y(_(E),((e,a)=>(u(),d("div",{key:a},[i("div",M,[w(me,null,{default:h((()=>[w(m,{span:16,class:"col-box-time"},{default:h((()=>[w(ne,{modelValue:e.startTime,"onUpdate:modelValue":a=>e.startTime=a,"max-time":e.endTime,placeholder:"起始时间",start:"00:00",step:"00:15",end:"24:00",size:"default",editable:!1},null,8,["modelValue","onUpdate:modelValue","max-time"]),v("至 "),w(ne,{modelValue:e.endTime,"onUpdate:modelValue":a=>e.endTime=a,"min-time":e.startTime,placeholder:"结束时间",start:"00:00",step:"00:15",end:"24:00",size:"default",editable:!1},null,8,["modelValue","onUpdate:modelValue","min-time"])])),_:2},1024),w(m,{span:8},{default:h((()=>[w(n,{modelValue:e.chargeP,"onUpdate:modelValue":a=>e.chargeP=a,placeholder:"功率值",size:"default",autocomplete:"off"},{default:h((()=>[F])),_:2},1032,["modelValue","onUpdate:modelValue"])])),_:2},1024)])),_:2},1024)])])))),128)),i("div",H,[w(p,{class:"newpower-btn",type:"primary",size:"default",onClick:oe},{default:h((()=>[v("设置")])),_:1}),w(p,{class:"newpower-btn",type:"primary",size:"default",onClick:se},{default:h((()=>[v("查看当前功率曲线")])),_:1})])])])),_:1})])),_:1}),w(m,{span:24},{default:h((()=>[i("div",K,[i("div",Z,[w(j,{chartseries:x.value},null,8,["chartseries"])])])])),_:1})])),_:1})])])}}}),[["__scopeId","data-v-54eb8e22"]]);export{q as default};

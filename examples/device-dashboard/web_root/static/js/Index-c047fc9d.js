import{u as e,b as l}from"./vue-router-d0599d8d.js";import{u as a,_ as s}from"./index-93c21a11.js";import{d as t,e as n,j as i,f as o,ag as r,c as u,U as d,O as c,o as p,M as m,u as f,F as v,a7 as g,T as y,R as x,a as _,J as k,S as h}from"./@vue-9d7d117a.js";import"./nprogress-867e68f1.js";import"./dayjs-9ee440cb.js";import"./pinia-c89a5039.js";import"./element-plus-1abe4ec3.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-a55f440f.js";import"./@element-plus-9c1020e5.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";/* empty css                    */const j={class:"admin"},b={key:0,class:"logo"},w={key:0},z={key:1,class:"logo"},C={key:0,style:{"padding-left":"16px"}},F={style:{"padding-left":"16px"}},I={key:0,style:{"padding-left":"16px"}},L={class:"display-flex ai-center jc-space-between flex-1 pl-20"},M={class:"display-flex ai-center ac-center"},T=["onClick"],$={class:"display-flex ai-center user"},E={class:"nav-menu-item"},J={class:"display-flex ai-center"},O={class:"el-dropdown-link cursor-pointer ml-6 fs-14"},P={style:{background:"#fff",padding:"20px"}},R=s(t({__name:"Index",setup(s){const t=a(),R=e();l();const S=n(!1),U=n(!0),q=i((()=>t.menu||[])),A=i((()=>t.breadcrumb||[]));o(S,(e=>{U.value=!U.value,setTimeout((()=>{U.value=!U.value}),100)}));const B=()=>{t.logout()};return(e,l)=>{const a=r("el-image"),s=r("svg-icon"),t=r("el-menu-item"),n=r("el-menu-item-group"),i=r("el-sub-menu"),o=r("el-menu"),D=r("el-aside"),G=r("Expand"),H=r("Fold"),K=r("el-icon"),N=r("el-breadcrumb-item"),Q=r("el-breadcrumb"),V=r("Monitor"),W=r("router-link"),X=r("arrow-down"),Y=r("el-dropdown-item"),Z=r("el-dropdown-menu"),ee=r("el-dropdown"),le=r("el-header"),ae=r("router-view"),se=r("el-scrollbar"),te=r("el-main"),ne=r("el-container");return p(),u("div",j,[d(ne,{class:"height-percent-100"},{default:c((()=>[d(D,{width:S.value?"65px":"220px",class:"aside scrollbar"},{default:c((()=>[U.value?(p(),u("div",b,[S.value?(p(),m(a,{key:1,style:{width:"55px"},src:f("/static/png/logonew-08aa3866.png")},null,8,["src"])):(p(),u("span",w,"正辉智慧终端"))])):(p(),u("div",z)),d(o,{"default-active":e.$route.path,collapse:S.value,class:"menu","background-color":"rgba(88, 89, 91, .9)","text-color":"rgba(255, 255, 255, .9)",router:"","collapse-transition":!1,"active-text-color":"#fff"},{default:c((()=>[(p(!0),u(v,null,g(q.value,((e,l)=>(p(),u(v,null,[e.childList.length?(p(),m(i,{key:l,index:e.url},{title:c((()=>[d(s,{name:e.icon},null,8,["name"]),S.value?x("",!0):(p(),u("span",C,y(e.name),1))])),default:c((()=>[d(n,null,{default:c((()=>[(p(!0),u(v,null,g(e.childList,((e,l)=>(p(),m(t,{key:l,index:e.url},{title:c((()=>[d(s,{name:e.icon},null,8,["name"]),_("span",F,y(e.name),1)])),_:2},1032,["index"])))),128))])),_:2},1024)])),_:2},1032,["index"])):(p(),m(t,{key:l+1,index:e.url},{default:c((()=>[d(s,{name:e.icon},null,8,["name"]),S.value?x("",!0):(p(),u("div",I,y(e.name),1))])),_:2},1032,["index"]))],64)))),256))])),_:1},8,["default-active","collapse"])])),_:1},8,["width"]),d(ne,null,{default:c((()=>[d(le,{class:"display-flex fd-column jc-center header"},{default:c((()=>[_("div",L,[_("div",M,[d(K,{class:"mr-12",size:24,onClick:l[0]||(l[0]=e=>S.value=!S.value)},{default:c((()=>[S.value?(p(),m(G,{key:0})):(p(),m(H,{key:1}))])),_:1}),d(Q,{"separator-class":"el-icon-arrow-right"},{default:c((()=>[(p(!0),u(v,null,g(A.value,((e,l)=>(p(),m(N,{key:l},{default:c((()=>[_("span",{class:"cursor-pointer",style:k({color:A.value.length-1===l?"#1990FF":"#999999"}),onClick:e=>(e=>{e+1!==A.value.length&&0!==e&&R.go(e+1-A.value.length)})(l)},y(e),13,T)])),_:2},1024)))),128))])),_:1})]),_("div",$,[d(W,{class:"h100",target:"_blank",title:"首页",to:"/"},{default:c((()=>[_("div",E,[d(K,{color:"#000",style:{"font-size":"20px"}},{default:c((()=>[d(V)])),_:1})])])),_:1}),d(ee,{size:"large",trigger:"click"},{dropdown:c((()=>[d(Z,null,{default:c((()=>[d(Y,{onClick:B},{default:c((()=>[h(" 退出登录 ")])),_:1})])),_:1})])),default:c((()=>[_("div",J,[_("span",O,[h(" 管理员"),d(K,{class:"el-icon--right"},{default:c((()=>[d(X)])),_:1})])])])),_:1})])])])),_:1}),d(te,{class:"scrollbar-y",style:{height:"calc(100vh - 140px)",background:"#f4f4f4"}},{default:c((()=>[d(se,null,{default:c((()=>[_("div",P,[(p(),m(ae,{key:e.$route.fullPath}))])])),_:1})])),_:1})])),_:1})])),_:1})])}}}),[["__scopeId","data-v-087c917a"]]);export{R as default};

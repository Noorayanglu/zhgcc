const e=new Map;e.set("温度","℃"),e.set("温差","℃"),e.set("电压","V"),e.set("压差","mV"),e.set("电流","A"),e.set("有功功率","kW"),e.set("功率设定","kW"),e.set("无功功率","kVar"),e.set("频率","Hz"),e.set("SOC","%"),e.set("soc","%"),e.set("SOH","%"),e.set("soh","%"),e.set("电阻","kΩ"),e.set("R+","kΩ"),e.set("R-","kΩ"),e.set("转速","RPM"),e.set("压力","Bar"),e.set("电能","kWh"),e.set("浓度","ppm");const t=t=>{let s="";for(let r of e.keys())if(t.endsWith(r)){s=e.get(r);break}return s},s=e=>{let s=[];for(let r in e){let n={};n.name=r,n.value=e[r],n.unit=t(r),s.push(n)}return s},r=e=>{if(isNaN(e))return e;return/e/i.test(""+e)?e.toFixed(18).replace(/\.?0+$/,""):e};export{r as g,s as j};
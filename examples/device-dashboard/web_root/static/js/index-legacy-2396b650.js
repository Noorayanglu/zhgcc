!function(){function t(t,r){var n="undefined"!=typeof Symbol&&t[Symbol.iterator]||t["@@iterator"];if(!n){if(Array.isArray(t)||(n=function(t,r){if(!t)return;if("string"==typeof t)return e(t,r);var n=Object.prototype.toString.call(t).slice(8,-1);"Object"===n&&t.constructor&&(n=t.constructor.name);if("Map"===n||"Set"===n)return Array.from(t);if("Arguments"===n||/^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n))return e(t,r)}(t))||r&&t&&"number"==typeof t.length){n&&(t=n);var o=0,a=function(){};return{s:a,n:function(){return o>=t.length?{done:!0}:{done:!1,value:t[o++]}},e:function(t){throw t},f:a}}throw new TypeError("Invalid attempt to iterate non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method.")}var i,u=!0,s=!1;return{s:function(){n=n.call(t)},n:function(){var t=n.next();return u=t.done,t},e:function(t){s=!0,i=t},f:function(){try{u||null==n.return||n.return()}finally{if(s)throw i}}}}function e(t,e){(null==e||e>t.length)&&(e=t.length);for(var r=0,n=new Array(e);r<e;r++)n[r]=t[r];return n}System.register([],(function(e,r){"use strict";return{execute:function(){var r=new Map;r.set("温度","℃"),r.set("温差","℃"),r.set("电压","V"),r.set("压差","mV"),r.set("电流","A"),r.set("有功功率","kW"),r.set("功率设定","kW"),r.set("无功功率","kVar"),r.set("频率","Hz"),r.set("SOC","%"),r.set("soc","%"),r.set("SOH","%"),r.set("soh","%"),r.set("电阻","kΩ"),r.set("R+","kΩ"),r.set("R-","kΩ"),r.set("转速","RPM"),r.set("压力","Bar"),r.set("电能","kWh"),r.set("浓度","ppm");var n=function(e){var n,o="",a=t(r.keys());try{for(a.s();!(n=a.n()).done;){var i=n.value;if(e.endsWith(i)){o=r.get(i);break}}}catch(u){a.e(u)}finally{a.f()}return o};e("j",(function(t){var e=[];for(var r in t){var o={};o.name=r,o.value=t[r],o.unit=n(r),e.push(o)}return e}))}}}))}();

<?xml version="1.0" encoding="WINDOWS-1251"?>
<file>
  <auth>{{oauth}}</auth>

  {{#order}}
  <order
    order_id = "{{order_id}}"
    
    date = "{{date}}"
    date_desired = "{{date_desired}}"
    
    sender_city = "{{sender_city}}"
    sender_company = "{{sender_company}}"
    sender_address = "{{sender_address}}"
    sender_contact = "{{sender_contact}}"
    sender_phone = "{{sender_phone}}"
    
    rcpt_city_name = "{{rcpt_city_name}}"
    rcpt_name = "{{rcpt_name}}"
    rcpt_warehouse = "{{rcpt_warehouse}}"
    rcpt_street_name = "{{rcpt_street_name}}"
    rcpt_contact = "{{rcpt_contact}}"
    rcpt_phone_num = "{{rcpt_phone_num}}"
    
    pay_type = "{{pay_type}}"
    payer = "{{payer}}"
    cost = "{{cost}}"

    payer_city = "{{payer_city}}"
    payer_company = "{{payer_company}}"

    additional_info = "{{additional_info}}"

    documents = "{{documents}}"
    pack_type = "{{pack_type}}"
    description = "{{description}}"
    floor_count = "{{floor_count}}"
    saturday = "{{saturday}}"

    delivery_amount = "{{delivery_amount}}"

    redelivery_type = "{{redelivery_type}}"
    delivery_in_out = "{{delivery_in_out}}"
    redelivery_payment_city = "{{redelivery_payment_city}}"
    redelivery_payment_payer = "{{redelivery_payment_payer}}"

    weight = "{{weight}}"
    length = "{{length}}"
    width = "{{width}}"
    height = "{{height}}"
  >
    <order_cont cont_description = "{{cont_description}}" />

  </order>
  {{/order}}

  
</file>

{{!




Query
<file>
  <auth>363e5b2b2fb02543a9cedc6e4f1470bc</auth>
  <order
    order_id="9950"
    sender_city="Харків"
    sender_company="ПП Доценко"
    sender_address="1"
    sender_contact="Олексій"
    sender_phone="0504044040"
    rcpt_city_name="Кіровоград"
    rcpt_name="Приватна особа"
    rcpt_warehouse="2"
    rcpt_contact="Гранатова Ольга"
    rcpt_phone_num="0505421214"
    pack_type="Коробка"
    description="Взуття"
    pay_type="1"
    payer="1"
    cost="200"
    weight="0,1">
    <order_cont
      cont_description="Кільце" />
  </order>
</file>

Answer
<?xml version="1.0" encoding="UTF-8"?>
<file>
  <order id="9949" np_id="[наш_номер_документа]" />
  <order id="9950" np_id="[наш_номер_документа]" />
</file>

}}

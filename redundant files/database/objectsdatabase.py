import boto3

def create_objects_table(dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb',region_name='us-east-1')

    table = dynamodb.create_table(
        TableName='Objects',
        KeySchema=[
            {
                'AttributeName': 'Object',
                'KeyType': 'HASH'  # Partition key
            },
            {
                'AttributeName': 'xyVals',
                'KeyType': 'RANGE'  # Sort key
            }
        ],
        AttributeDefinitions=[
            {
                'AttributeName': 'Object',
                'AttributeType': 'N'
            },
            {
                'AttributeName': 'xyVals',
                'AttributeType': 'N'
            },

        ],
        ProvisionedThroughput={
            'ReadCapacityUnits': 100,
            'WriteCapacityUnits': 100
        }
    )
    return table


if __name__ == '__main__':
    objects_table = create_objects_table()
    print("Table status:", objects_table.table_status)
